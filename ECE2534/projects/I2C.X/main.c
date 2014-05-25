// ECE 2534:        OLEDtest
// Purpose:         Test the Digilent board by writing a short message to the OLED.
//                  Also display a counter that updates every millisecond.
// Resources:       main.c uses Timer2 to generate interrupts at intervals of 1 ms.
//					delay.c uses Timer1 to provide delays with increments of 1 ms.
//					PmodOLED.c uses SPI1 for communication with the OLED.
// Written by:      CDP (and modified by ALA)
// Last modified:   1/31/2014
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

// Global variables
unsigned sec1000 = 0; // This is updated 1000 times per second by the interrupt handler

// Interrupt handler - respond to timer-generated interrupt
void __ISR(_TIMER_2_VECTOR, ipl1) InterruptHandler_2534( void )
{
   if( INTGetFlag(INT_T2) )            // Verify source of interrupt
   {
      sec1000++;                        // Update global variable
      INTClearFlag(INT_T2);             // Acknowledge interrupt
   }
}

// Added shortcuts for readability
#define BIT_6   (1 << 6)
#define BIT_7   (1 << 7)
#define BIT_8   (1 << 8)
#define BIT_9   (1 << 9)
#define BIT_10  (1 << 10)
#define BIT_11  (1 << 11)
#define BIT_12  (1 << 12)
#define BIT_13  (1 << 13)
#define BIT_14  (1 << 14)
#define BIT_15  (1 << 15)


int main()
{
   char buf[17];        // Temp string for OLED display

   // Initialize GPIO for BTN1, BTN2 and LED's
   TRISGCLR = BIT_12 | BIT_13 | BIT_14 | BIT_15;
   ODCGCLR  = 0x1000;   // For LED1: configure as normal output(not open drain)
   TRISGSET = BIT_6 | BIT_7;     // For BTN1: configure PortG bit for input

   // Initialize delay timer and OLED
   DelayInit();
   OledInit();

   // Set up timer 2 to roll over every ms
   OpenTimer2(T2_ON         |
             T2_IDLE_CON    |
             T2_SOURCE_INT  |
             T2_PS_1_16     |
             T2_GATE_OFF,
             625);  // freq = 10MHz/16/625 = 1 kHz

   // Set up CPU to respond to interrupts from Timer2
   INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_1);  // set priority for an interupt
   INTClearFlag(INT_T2);                                            //clear interupt flag
   INTEnable(INT_T2, INT_ENABLED);                                  // enable an interupt source
   INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);               //enable single/multi vector mode
   INTEnableInterrupts();                                           // enable CPU

   // Send a welcome message to the OLED display
   OledClearBuffer();
   OledSetCursor(0, 0);
   OledPutString("ECE 2534");
   OledSetCursor(0, 2);
   // Changed it to my name
   OledPutString("conor atrick");
   OledUpdate();

   // Infinite loop
   while (1)
   {
      // Display millisecond count value
      sprintf(buf, "%16d", sec1000);
      OledSetCursor(0, 3);
      OledPutString(buf);
      OledUpdate();

      // Update LED's
      
      if(   // exclusive or
              PORTG & BIT_6 && !(PORTG & BIT_7) ||
              !(PORTG & BIT_6) && PORTG & BIT_7
        ){      // Check BTN1 and BTN2
         PORTGSET = BIT_12 | BIT_13 | BIT_14 | BIT_15; // everything on
      }
      else{
         PORTGCLR = BIT_12 | BIT_13 | BIT_14 | BIT_15; // everything off
      }
   }

   return 0;
}

void sendStart(I2C_MODULE i2c){
        while( !!! I2CBusIsIdle(i2c));
        I2CStart(i2c);
        I2CGetStatus(i2c);
        while( !!! (I2CGetStatus(i2c) & I2C_START) );
}

BYTE readByte(I2C_MODULE i2c){
    I2CReceiverEnable(i2c, TRUE);
    while ( !!! I2CReceivedDataIsAvailable(i2c));
    BYTE t;
    t = I2CGetByte(i2c);
    I2CAcknowledgeByte(i2c, FALSE);
    while( !!! I2CAcknowledgeHasCompleted(i2c));
    return t;
}
