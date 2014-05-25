/* 
 * File:   main.c
 * Author: Conor Patrick
 *
 * This code meets the requirements for the assignment.
 * 
 * Created on February 16, 2014, 3:56 PM
 */

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

int AVAL;


/*
    Interupt Handler for the ADC conversion
*/
void __ISR(_ADC_VECTOR, ipl2) InterruptHandler1_2534( void )
{        
    if( INTGetFlag(INT_AD1) )            // Verify source of interrupt
   {
      AVAL = ReadADC10(0);
      INTClearFlag(INT_AD1);             // Acknowledge interrupt
   }
}

void beginMyOled();
void displayOnOLED(char *str, int row);
float map(float x1, float x2, float y1, float y2, float val);

int main() {
    // start OLED
    
    beginMyOled();

    // 10ms Period.
    OpenTimer3(T3_ON         |          // timer 3 on
             T3_IDLE_CON    |           // don't stop on idle
             T3_SOURCE_INT  |           // Use internal clock
             T3_PS_1_16     |           // Prescale = 16
             T3_GATE_OFF,               // No gate control on timer
             6250);                      // counter register
    
    
    char buf[17] = "                ";
    /*
     Start ADC on, with signed 32 bit int format, started by timer 3,
     samples continuously
    */
    CloseADC10();
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN2 );
    OpenADC10(
            // turn on converter, 32 bit int format, start conversions with timer, sample the signal
            ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_TMR |ADC_AUTO_SAMPLING_ON,
            // standard Vref & Vdd, no scanning,  1 sample per interupt, use regular input
            ADC_VREF_AVDD_AVSS | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_1,
            // fast conversion clock, use internal clock
            ADC_CONV_CLK_Tcy2|ADC_SAMPLE_TIME_0,
            // enable AN2 for analog
            ENABLE_AN2_ANA,
            // no scanning
            SKIP_SCAN_ALL
    );
    
    

    ConfigIntADC10(
            ADC_INT_PRI_2|  // set priority
            ADC_INT_ON      // turn on interrupt
            );
    
    EnableADC10();          // turn on converter
    
    INTSetVectorPriority(INT_ADC_VECTOR, INT_PRIORITY_LEVEL_2);      // set priority for an interupt
    INTClearFlag(INT_AD1);                                           //clear interupt flag
    INTEnable(INT_AD1, INT_ENABLED);                                 // Enable ADC interupt
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);               //enable single/multi vector mode
    INTEnableInterrupts();                                           //enable interupts

    int lastAVAL;

    // Display a reading whenever it changes.
    while(1){

        if (AVAL != lastAVAL){
            sprintf( buf, "Voltage: %.2f",
                     map(0, 1024, 0, 3.3, (float)AVAL)
                    );
            displayOnOLED(buf,0);
            lastAVAL = AVAL;
        }
        
    }
    return (EXIT_SUCCESS);
}


/*
    Displays a message on the OLED
 */

void displayOnOLED(char *str, int row){
    if (!*str) return;
    OledSetCursor(0, 2+row);
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
   OledPutString("Conor Patrick");
   OledUpdate();
}

/*
    Converts a value from one range to another.
    First range is current and second range is desired.
 */
float map(float x1, float x2, float y1, float y2, float val){
    float r1 = val/(x2-x1);
    return (y2-y1)*(r1);
}
