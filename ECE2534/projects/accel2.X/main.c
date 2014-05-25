/* 
 * File:   main.c
 * Author: Conor Patrick
 *
 * This code meets the requirements for the assignment.
 * 
 * Created on April 16, 2014, 3:56 PM
 */
#define _PLIB_DISABLE_LEGACY
#include <plib.h>
#include "PmodOLED.h"
#include "OledChar.h"
#include "OledGrph.h"
#include "delay.h"

// Digilent board configuration
#pragma config ICESEL       = ICS_PGx1  // ICE/ICD Comm Channel Select
#pragma config DEBUG        = OFF       // Debugger Disabled for Starter Kit

#pragma config FNOSC        = PRIPLL	// Oscillator selection
#pragma config POSCMOD      = XT	// Primary oscillator mode
#pragma config FPLLIDIV     = DIV_2	// PLL input divider
#pragma config FPLLMUL      = MUL_20	// PLL multiplier
#pragma config FPLLODIV     = DIV_1	// PLL output divider
#pragma config FPBDIV       = DIV_8	// Peripheral bus clock divider
#pragma config FSOSCEN      = OFF	// Secondary oscillator enable

#define THECLOCKHERTZ 10000000
/*
 Object for accelerometer
 */
typedef struct{
    int axis[3];
    int I2C;
    int ID;
    unsigned int read;
    unsigned int write;
} Accel;

void beginMyOled();
void displayOnOLED(char *str, int row);

int map(int v1, int v2, int r1, int r2, int val);
int abs(int a){ return (a < 0 ? a*-1 : a); }

void sendStart(I2C_MODULE i2c);
void repeatStart(I2C_MODULE I2C);
BYTE readByte(I2C_MODULE i2c);
void sendByte(I2C_MODULE i2c, BYTE byte);

void initI2CAccel(Accel *a, I2C_MODULE i2c);
unsigned int accelReadReg(const Accel *a, int reg);
void accelWriteReg(const Accel *a, int reg, BYTE val);
void readAccel(Accel *a);

int main() {    
    char buf[17] = "                ";
    
    Accel accel;
    beginMyOled();
    initI2CAccel(&accel, I2C2);
    // show ID
    sprintf(buf, "ID: %x", accelReadReg((const)&(accel), 0x0));
    displayOnOLED(buf,0);

    int data[3];
    unsigned int i, dec;
    while (TRUE){
        if (! ( accelReadReg((const)&(accel), 0x30) & BIT_7) )
                continue;                           // only update when data is ready.
        
        readAccel(&accel);
        for (i=0; i<3; i++)                         // convert to g * 100
            data[i] = map(0,0xff,0,100,accel.axis[i]);

        for (i=0; i<3; i++){
            if (data[i] < 0){
                data[i] *= -1;                      // show as decimal
                sprintf(buf, "%c: -%d.%02d g", i+0x58, data[i]/100, (data[i] - data[i]/100));
            }else                      // (ascii) 0x58 = X
                sprintf(buf, "%c: %02d.%02d g", i+0x58, data[i]/100, (data[i] - data[i]/100));
            displayOnOLED(buf,i+1);
        }

    }

    return (EXIT_SUCCESS);
}


/*
    Displays a message on the OLED
 */

void displayOnOLED(char *str, int row){
    if (!*str) return;
    OledSetCursor(0, row);
    OledPutString(str);
}

/*
    Initializes the OLED for debugging purposes
*/
void beginMyOled(){
   DelayInit();
   OledInit();

   OledSetCursor(0, 0);
   OledClearBuffer();
   OledUpdate();
}

/*
 Initializes the accelerometer and it's I2C channel
 */
void initI2CAccel(Accel *a, I2C_MODULE i2c){
    a->read = 0x3b;                     // read accel device value
    a->write = 0x3a;                    // write accel device value

    a->I2C = i2c;                       // I2C channel for accel device

    I2CSetFrequency(a->I2C,             //100 Hz
                    10*1000*1000, 100);
    I2CEnable(a->I2C, 1);               // turn on
    accelWriteReg(a, 0x2d, 0x8);        // set to measure mode
}

/*
 Reads important values from ADXL345 and writes them to Accel object
 */
void readAccel(Accel *a){

    a->axis[0] = accelReadReg((const) a, 0x33) - accelReadReg((const) a, 0x32);
    a->axis[1] = accelReadReg((const) a, 0x35) - accelReadReg((const) a, 0x34);
    a->axis[2] = accelReadReg((const) a, 0x37) - accelReadReg((const) a, 0x36);
     
}
/*
 Write a value to a register on ADXL345
 */
void accelWriteReg(const Accel *a, int reg, BYTE val){
    int I2C = a->I2C;
    sendStart(I2C);                 // start transaction
    sendByte(I2C, a->write);        // write accel device
    sendByte(I2C, reg);             // accel device register
    sendByte(I2C, val);             // value
    I2CStop(I2C);
}
/*
 Reads a value from a ADXL345 register and returns it
 */
unsigned int accelReadReg(const Accel *a, int reg){
    int I2C = a->I2C;
    sendStart(I2C);                 // start transaction
    sendByte(I2C, a->write);        // write accel device
    sendByte(I2C, reg);             // specify device register
    repeatStart(I2C);
    sendByte(I2C, a->read);         // read accel device
    unsigned int d = readByte(I2C);          // grab data
    I2CAcknowledgeByte(I2C, 0);     // Send nack
    while(!I2CAcknowledgeHasCompleted(I2C));
    I2CStop(I2C);                   // end transaction
    return d;
}

/*
 Sends a byte on an i2c channel (with no strings attached)
 */
void sendByte(I2C_MODULE i2c, BYTE byte){
    I2CSendByte(i2c, byte);                     // Send and wait for finish
    while( ! I2CTransmissionHasCompleted(i2c));
    while( ! (I2CByteWasAcknowledged(i2c)));    // wait for ack
}

/*
 Reads a byte received from i2c channel (with no strings attached)
 */
BYTE readByte(I2C_MODULE i2c){

    I2CReceiverEnable(i2c, TRUE);               // enable receiving data
    while ( ! (I2CReceivedDataIsAvailable(i2c)));

    BYTE t;
    t = I2CGetByte(i2c);                        // grab byte

    I2CReceiverEnable(i2c, FALSE);              // put it back
    return t;
}

/*
 Sends a start inside i2c transaction (with no strings attached)
 */
void repeatStart(I2C_MODULE I2C){
    I2CRepeatStart(I2C);
    while( ! (I2CGetStatus(I2C) & I2C_START) );
    I2CClearStatus(I2C, I2C_START);
}

/*
 Sends a start to begin i2c transaction (with no strings attached)
 */
void sendStart(I2C_MODULE i2c){
        while( ! I2CBusIsIdle(i2c));
        DelayMs(2);                     // timing for ADXL345
        I2CStart(i2c);
        while( ! (I2CGetStatus(i2c) & I2C_START) );
        I2CClearStatus(i2c, I2C_START);
}

/*
 Maps a value to a particular range
*/
int map(int v1, int v2, int r1, int r2, int val){
    return (((val*1000/(v2-v1)) * (r2-r1)))/1000;
}