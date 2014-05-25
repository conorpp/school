/* 
 * File:   main.c
 * Author: Conor Patrick
 *
 * This code meets the requirements for the assignment.
 * 
 * Created on February 16, 2014, 3:56 PM
 */
#define _PLIB_DISABLE_LEGACY
#include <plib.h>
#include "PmodOLED.h"
#include "OledChar.h"
#include "OledGrph.h"
#include "delay.h"
#include <time.h>
#include <stdlib.h>

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

long unsigned int MS_STAMP = 0;

// Interrupt handler - respond to timer-generated interrupt
void __ISR(_TIMER_2_VECTOR, ipl1) InterruptHandler_2534( void )
{
   if( INTGetFlag(INT_T2) )            // Verify source of interrupt
   {
      MS_STAMP++;                        // Update global variable
      INTClearFlag(INT_T2);             // Acknowledge interrupt
   }
}

enum{init=0, starting, fruiting, shooting, bomb, stat, over, wait} state;

void myDisplayOnOLED(char *str, int column);
void openMyUart(UART_MODULE theUART);
void beginMyOled();
void beginMyTimer();
void initFruitNinja();
void displayTime(int ms);
void sendMessage(char* msg, UART_MODULE theUART);
char* getFruit();
char readButton();


int main() {
    TRISGSET = BIT_6 | BIT_7;     // For BTN1: configure PortG bit for input

    /*
     Start OLED, UART, TIMER, FruitNinja
     Seed random function with time
     Init state
    */
    beginMyOled();
    openMyUart(UART1);
    beginMyTimer();
    initFruitNinja();
    srand(time(NULL));          
    state = init;
    
    char c;                                 // data
    char buf[17] = "                ";      // OLED buffer
    char bigbuf[60] = "                                                            ";
    int lastTimerSec= 0 ,lastFruitSec = 0, startTime=0, sec = 0;    // Timestamps
    int deltaT = 8;                         // game variables
    int statTime = 2;
    int numOfFruit = 5;
    int shots = 0, goal = 0, shotTimeStamp = 0, responseTime=0;
    int slicedFruits = 0, missedFruits = 0, avgResponse = 0;
    int bombed = 0, bombs = 0;


    while(1){

        // Update OLED once a second
        sec = MS_STAMP / 1000;
        if (sec > lastTimerSec) {
            displayTime(MS_STAMP);
            lastTimerSec = sec;
        }
        
        // Button Press
        c = readButton();
        
        // State machine
        switch (state){
            case init:
                if ( !(c == '1') )break;            // check if start of game
                sendMessage("NEW GAME BEGINS!", UART1);
                state = starting;
                startTime = sec;
            break;
            case starting:
                if ( sec - startTime >= 5)    // wait 5s
                    state = fruiting;
            break;
            case fruiting:                          // dish out a fruit or bomb
                if (!numOfFruit) {
                    state = over;
                    sendMessage("!!!!GAME OVER!!!!", UART1);
                    break;
                }
                numOfFruit--;
                shotTimeStamp = MS_STAMP;
                int chance = ( rand() % 10 )+1;
                if(chance < 7){
                    goal = ( rand() % 4 )+1;
                    char *fruit = getFruit();
                    sprintf(buf, "%d %s", goal, fruit);
                    state = shooting;
                }else{
                    sprintf(buf, "BOMB     ");
                    state = bomb;
                    bombs++;
                }
                sendMessage(buf, UART1);
                lastFruitSec = sec;

            break;
            case shooting:                      // count shoots for period of time
                if (sec - lastFruitSec >= deltaT) {
                    state = stat;
                    lastFruitSec = sec;
                    // Response time if fruit was sliced.  
                    int resp = (shots >= goal) ? responseTime : MS_STAMP - shotTimeStamp;
                    avgResponse += resp;
                    if (shots >= goal) slicedFruits++;
                    else missedFruits++;
                    sprintf(buf, "-----Sliced %d fruits in %d ms-----",
                            shots, (resp));
                    sendMessage(buf, UART1);
                    break;
                }else
                    if (c == '1'){
                        shots++;
                        if (shots >= goal){
                            responseTime = MS_STAMP - shotTimeStamp;
                            shots = goal;
                        }
                    }


            break;                           // watch bomb for period of time
             case bomb:
                if (sec - lastFruitSec >= deltaT) {
                    state = fruiting;
                    lastFruitSec = sec;
                    break;
                }else
                    if (c == '1'){
                        sendMessage("!!!!BOOOOMMMM!!!!", UART1);
                        int bombed = 1;
                        lastFruitSec = sec;
                        state = over;
                        sendMessage("!!!!GAME OVER!!!!", UART1);
                    }
            break;
            case stat:                          // display stats for 2s
                if (sec - lastFruitSec >= statTime) {
                    state = fruiting;
                    lastFruitSec = sec;
                    shots = 0;
                    break;
                }

            break;
            case over:                               // start over
                if (sec - lastFruitSec < 5)break;    // display stats after 5s
                sprintf(bigbuf, "AVERAGE RESPONSE TIME = %d ms",
                    avgResponse/(missedFruits + slicedFruits));
                sendMessage(bigbuf, UART1);
                sprintf(bigbuf, "TOTAL FRUITS SLICED = %d ",
                    slicedFruits);
                sendMessage(bigbuf, UART1);
                sprintf(bigbuf, "MISSED FRUITS = %d ",
                    missedFruits);
                sendMessage(bigbuf, UART1);

                // adjust time for next round.
                if (!bombed && !missedFruits) deltaT = deltaT/2;
                slicedFruits = missedFruits = avgResponse = 0;
                startTime = sec;
                numOfFruit = 5;
                bombed = bombs = 0;
                state = wait;
                lastFruitSec = sec;
                
            break;
            case wait:
                if (sec - lastFruitSec < 5) break;
                lastFruitSec = sec;
                state = init;
                initFruitNinja();
            break;
        }


        // display the data on the OLED
        sprintf(buf,"%c         ",c);
        myDisplayOnOLED(buf,0);
        
    }
    return (EXIT_SUCCESS);
}


/*
    Displays a message on the OLED
 */
void myDisplayOnOLED(char *str, int column){
    if (!*str) return;
    OledSetCursor(0, 2+column);
    OledPutString(str);
}

/*
    Starts a timer for project
 */
void beginMyTimer(char *str){

    OpenTimer2(T2_ON         |          // timer 2 on
             T2_IDLE_CON    |           // don't stop on idle
             T2_SOURCE_INT  |           // Use internal clock
             T2_PS_1_16     |           // Prescale = 16
             T2_GATE_OFF,               // No gate control on timer
             625);                      // counter register
    // freq = 10MHz/16/625 = 1 kHz

   // Set up CPU to respond to interrupts from Timer2
   INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_1);  // set priority for an interupt
   INTClearFlag(INT_T2);                                            //clear interupt flag
   INTEnable(INT_T2, INT_ENABLED);                                  // enable an interupt source
   INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);               //enable single/multi vector mode
   INTEnableInterrupts();                                           // enable CPU
}

/*
    Opens a uart with the general settings
 */
void openMyUart(UART_MODULE theUART){
    UARTConfigure( theUART, UART_ENABLE_PINS_TX_RX_ONLY );
    UARTSetDataRate(theUART, THECLOCKHERTZ, 9600);
    UARTEnable( theUART, UART_ENABLE|UART_PERIPHERAL|UART_RX|UART_TX );
}



/*
    Initializes the OLED for debugging purposes
*/
void beginMyOled(){
   DelayInit();
   OledInit();

   OledSetCursor(0, 0);
   OledClearBuffer();
   OledPutString("Conor Patrick");
   OledUpdate();
}

/*
     Displays the time in hr:min:sec format on OLED
     for a MS timestamp.
 */
void displayTime(int ms){
    char buf[17] = "                ";
    int secs = ms / 1000;
    int min = secs / 60;
    int hours = min / 60;
    
    sprintf(buf, "%02d : %02d : %02d",
            hours,
            min % 60,
            secs % 60
            );
    myDisplayOnOLED(buf,1);
}
/*
    Starts off game by sending the user a message
 */
void initFruitNinja(){
    sendMessage("Press button 1 to start Fruit Ninja", UART1);
}


/*
    Sends a string to specified UART on a new line.
 */
void sendMessage(char* msg, UART_MODULE theUART){
    int i =0;
    UARTSendDataByte(theUART, '\n');        // new line
    UARTSendDataByte(theUART, '\r');
    while(*msg){
        while( !UARTTransmissionHasCompleted(theUART) );        // wait for uart to be ready
        UARTSendDataByte(theUART, *msg);
        msg++;
    }
}

/*
    Returns a pointer to a fruit string (random)
*/
char* getFruit(){
    const char *fruits[] = {
           "STRAWBERRY","APPLE","PEAR",
           "BANANA","PINEAPPLE","GRAPE",
           "COCONUT","MANGO","WATERMELON",
           "HONEYDEW","PEACH","CHERRY",
           "STARFRUIT","PAPAYA","RASPBERRY",
           "BLUEBERRY"
    };
    return fruits[ ( rand() % 16 ) ];
}


/*
 Returns which button is being pressed as a char.
 returns '\0' when not;
*/

char readButton(){
    if ( PORTG &  BIT_6){
        PORTGSET = BIT_12 | BIT_13 | BIT_14 | BIT_15; // everything on
        return '1';
    }else{
        PORTGCLR = BIT_12 | BIT_13 | BIT_14 | BIT_15; // everything off
        return '0';
    }
}


