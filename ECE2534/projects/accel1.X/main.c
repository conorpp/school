/* 
 * File:   main.c
 * Author: Conor Patrick
 *
 * This code meets the requirements for the assignment.
 * 
 * Created on April 7, 2014, 3:56 PM
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


/*
 Object for accelerometer
 */
typedef struct{
    int X;
    int Y;
    int Z;
    unsigned int ID;
} Accel;


void beginMyOled();
void displayOnOLED(char *str, int row);
Accel * readAccel(SpiChannel chn);
void initAccel(SpiChannel chn);
int map(int v1, int v2, int r1, int r2, int val);

// slave select
int SS = BIT_12;

int main() {
    
    // start OLED 
    beginMyOled();
    TRISFCLR = BIT_12;

    
    char buf0[17] = "                ";
    char buf1[17] = "                ";
    char buf2[17] = "                ";
    char buf3[17] = "                ";
    int cc = SPI_CHANNEL4;
    // open the SPI channel for accelerometer
    
    SpiChnOpen(cc,
            // master mode, idle high, transmit from idle to active
            SPI_OPEN_MSTEN | SPI_OPEN_CKP_HIGH |  
            // 8 bits, turn spi on, enhanced buffer mode
            SPI_OPEN_MODE8  |SPI_OPEN_ENHBUF ,
            // set bitrate to 10MHz/20 = .5 MHz
            10
    );
    
    initAccel(cc);
    // start transmission by setting SS low
    Accel * data;
    int x ,y,z;
    while (TRUE){
        // Grab new data
        data = readAccel(cc);
        
        sprintf(buf0, "id: %x", data->ID);
        sprintf(buf1, " X: %06x", data->X);
        sprintf(buf2, " Y: %06x", data->Y);
        sprintf(buf3, " Z: %06x", data->Z);
        // Map to g


        // Display without floating point
        /*
        x = map(0,255,0,200, data->X);
        y = map(0,255,0,200, data->Y);
        z = map(0,255,0,200, data->Z);
        sprintf(buf1, " X: %d.%02d g", x/100, (x - (x/100)*100));
        sprintf(buf2, " Y: %d.%02d g",y/100, (y - (y/100)*100));
        sprintf(buf3, " Z: %d.%02d g", z/100, (z - (z/100)*100));
         */
        displayOnOLED(buf0,0);
        displayOnOLED(buf1,1);
        displayOnOLED(buf2,2);
        displayOnOLED(buf3,3);

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

void initAccel(SpiChannel chn){


    
    PORTFCLR = BIT_12;   // GRAB THE SLAVE

    // read.  Send 0 bit for write command. 0 for multibyte.
    // (read/write << 7) | (multi/single bit << 6) | (address)
    SpiChnPutC(chn,( (BIT_7 & 0) | (BIT_6 & 0) | (0x2c)));
    SpiChnPutC(chn,( 0x8 ));    // data rate to 25 Hz
    while(SpiChnIsBusy(chn));
    SpiChnPutC(chn,( (BIT_7 & 0) | (BIT_6 & 0) | (0x2d)));
    SpiChnPutC(chn,( 0x8 ));    // set to measure mode
    while(SpiChnIsBusy(chn));
    PORTFSET = BIT_12;  // RELEASE THE SLAVE
}
/*
 Reads a register value with SPI
 */
int _SPI_readReg(SpiChannel chn, int addr){
    PORTFCLR = BIT_12;   // GRAB THE SLAVE
    // read.  Send 1 bit for read command. 0 for multibyte.
    // (read/write << 7) | (multi/single bit << 6) | (address)
    SpiChnGetRov(chn, 1);
    SpiChnPutC(chn,( (BIT_7) | (addr)));
    SpiChnGetRov(chn, 1);
    SpiChnPutC(chn,( 0xff));
    while(SpiChnIsBusy(chn));
    SpiChnGetRov(chn, 1);
    PORTFSET = BIT_12;  // RELEASE THE SLAVE
    return SpiChnGetC(chn);
}
/*
 Returns a pointer to a Accel struct packed with data.
 */
Accel * readAccel(SpiChannel chn){
    static Accel result;

   
    result.ID = _SPI_readReg(chn, 0x0);
    
    result.X = _SPI_readReg(chn, 0x32);

    result.Y = _SPI_readReg(chn, 0x34);
    
    result.Z = _SPI_readReg(chn, 0x36) ;
    
    
    return &result;
};
/*
Maps a value to a particular range
*/
int map(int v1, int v2, int r1, int r2, int val){
    return (((val*1000/(v2-v1)) * (r2-r1)))/1000;
}