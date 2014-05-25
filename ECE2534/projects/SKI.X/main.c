/* 
 * File:   main.c
 * Author: Conor Patrick
 *
 * This code meets the requirements for the assignment.
 * Please note I wrote a header file "myheader.h"
 * 
 * Created on February 16, 2014, 3:56 PM
 */
#define _PLIB_DISABLE_LEGACY
#include <plib.h>
#include <stdlib.h>
#include "PmodOLED.h"
#include "OledChar.h"
#include "OledGrph.h"
#include "delay.h"
#include "myheader.h"

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

void initOptions(){
    /* init stats for game */
    SKI.ADC = 0;
    SKI.time = 0;
    SKI.playFieldWidth = 0;
    SKI.hPos = 5*8;
    SKI.hPosMax = (9*8)+2;
    SKI.vPos = 3*8;
    SKI.vPosMax = 3*8;
    SKI.newGame = 1;
    SKI.level = 0;
    SKI.score = 0;
    SKI.gateThreshold = 3;
    SKI.isFlashing = 0;
    SKI.flashOn = 0;
    
    SKI.skiCode = 0;
    SKI.treeCode = 1;
    SKI.gateCode = 2;
    SKI.zombieCode = 3;
    SKI.codeOffset = 4;
    

    SKI.fallTime = 500;
    SKI.throwTime = 2000;
    SKI.refreshTime = 20;
    SKI.initTime = 5000;
    SKI.gameOverTime = 5000;
    SKI.flashTime = 100;
    SKI.flashingTime = SKI.flashTime * 8;

    SKI.flashing = SKI.flash = SKI.initT = SKI.refresh = SKI.throwT = SKI.fall = 0;
    
    /*   Register custom characters   */
    BYTE _SKI[] = {0,0,0xc0,0xf0,0xf0,0xc0,0,0};
    BYTE _TREE1[] = {1,1,0x02,0x0f,0x07,0x02,1,1};
    BYTE _GATE1[] = {0x07,0,0x0,0x0,0x0,0x0,0,0x07};
    BYTE _ZOMBIE1[] = {0x0c,0x03,0x05,0x09,0x09,0x05,0x03,0x0c};
    BYTE _TREE2[] = {0x10,0x10,0x20,0xf0,0x70,0x20,0x10,0x10};
    BYTE _GATE2[] = {0x70,0,0x0,0x0,0x0,0x0,0,0x70};
    BYTE _ZOMBIE2[] = {0xc0,0x30,0x50,0x90,0x90,0x50,0x30,0xc0};
    OledDefUserChar(SKI.skiCode, _SKI);
    OledDefUserChar(SKI.treeCode, _TREE1);
    OledDefUserChar(SKI.gateCode, _GATE1);
    OledDefUserChar(SKI.zombieCode, _ZOMBIE1);
    OledDefUserChar( SKI.treeCode+SKI.codeOffset , _TREE2);
    OledDefUserChar( SKI.gateCode+SKI.codeOffset , _GATE2);
    OledDefUserChar( SKI.zombieCode+SKI.codeOffset , _ZOMBIE2);
    
    /* init the objects (trees or gates) */
    static Object _objs[] = { {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0} };
    SKI.objects = _objs;
    SKI.objectsLength = 5;

    SKI.state = init;
}
/*
    Interupt Handler for the ADC conversion
*/
void __ISR(_ADC_VECTOR, ipl2) ADC_HANDLER( void )
{        
    if( INTGetFlag(INT_AD1) )            // Verify source of interrupt
   {
      SKI.ADC = ReadADC10(0);
      INTClearFlag(INT_AD1);             // Acknowledge interrupt
   }
}
/*
    Interupt Handler for Timer3
*/
void __ISR(_TIMER_3_VECTOR, ipl2) TIMER3_HANDLER( void )
{
    if( INTGetFlag(INT_T3) )            // Verify source of interrupt
   {
      SKI.time += 10;
      INTClearFlag(INT_T3);             // Acknowledge interrupt
   }
}

void beginMyOled();
void beginMyTimer3();
void beginMyADC();
void beginMyInterupts();

void SendCharToOled(int line, int pos, char str);
void printFatFrame(FatFrame *frame, int offset);
void setScore(int s);
void setLevel();
void moveLeft();
void moveRight();
void initTree();
void reset(int *t){*t = SKI.time;}
void throwRandomObject();
void throwObject(Object *obj, int code);
void checkObject(Object *obj);
void checkAllObjects();
int objectExists(int Abit);
void resetObjects();
int readButton();

int main() {

    beginMyOled();
    beginMyTimer3();
    beginMyADC();
    beginMyInterupts();
    initOptions();
    TRISGSET = BIT_6;     // Configure PortG bit for input
    
    char buf1[] = "                 ";
    char buf2[] = "                 ";
    FatFrame initFrame;
    initFrame.row0 = "                ";
    initFrame.row1 = "XtReme Downhill!";
    initFrame.row2 = "Conor Patrick   ";
    initFrame.row3 = "                ";
    FatFrame rightSide;
    rightSide.row0 = "          |LEVEL";
    rightSide.row1 = "          |  000";
    rightSide.row2 = "          |SCORE";
    rightSide.row3 = "          |  000";
    FatFrame overFrame;
    overFrame.row0 = "      OUCH!!    ";
    overFrame.row1 = "                ";
    overFrame.row2 = "Score = %d      ";
    overFrame.row3 = "Level = %d      ";
    FatFrame flashFrame;
    flashFrame.row0 = "XXXXXXXXXXXXXXXX";
    flashFrame.row1 = "XXXXXXXXXXXXXXXX";
    flashFrame.row2 = "XXXXXXXXXXXXXXXX";
    flashFrame.row3 = "XXXXXXXXXXXXXXXX";

    SendCharToOled(SKI.vPos, SKI.hPos, SKI.skiCode);
    
    while(1){

        if (SKI.time - SKI.refreshTime > SKI.refresh){
            reset(&SKI.refresh);
            switch(SKI.state){
                /* Start Game */
                case init:
                    if (SKI.time - SKI.initTime < SKI.initT){
                        if (SKI.newGame) printFatFrame(&initFrame, 0);
                        SKI.newGame = 0;
                    }else{
                        reset(&SKI.initT);
                        printFatFrame(&rightSide, 0);
                        SKI.state = play;
                    }
                break;
                /* Move around, throw objects  */
                case play:
                    if (SKI.ADC < 400){
                        moveLeft();
                    }else if (SKI.ADC > 600){
                        moveRight();
                    }else SendCharToOled(SKI.vPos, SKI.hPos, SKI.skiCode);

                    if (SKI.time - SKI.fallTime > SKI.fall){
                        reset(&SKI.fall);
                        throwRandomObject();
                        checkAllObjects();
                    }
                break;
                /* Flash screen frames */
                case flash:
                    if (!SKI.isFlashing){
                        SKI.flashing = SKI.time;
                        SKI.isFlashing = 1;
                    }
                    if (SKI.time - SKI.flashingTime < SKI.flashing){
                        if (SKI.time - SKI.flashTime > SKI.flash){
                            reset(&SKI.flash);
                            SKI.flashOn = !SKI.flashOn;
                            if (SKI.flashOn) printFatFrame(&flashFrame,0);
                            else OledClear();
                        }
                    }else{
                        reset(&SKI.flash);
                        SKI.state = gameover;
                    }
                break;
                /* Show stats and start over */
                case gameover:
                    if (!SKI.newGame) reset(&SKI.gameOver);
                    if (SKI.time - SKI.gameOverTime < SKI.gameOver){
                        if (!SKI.newGame){
                            OledClear();
                            sprintf(buf1, "Score = %d      ", SKI.score);
                            overFrame.row2 = buf1;
                            sprintf(buf2, "Level = %d      ", SKI.level);
                            overFrame.row3 = buf2;
                            printFatFrame(&overFrame, 0);
                        }
                        SKI.newGame = 1;
                    }else{
                        reset(&SKI.gameOver);
                        reset(&SKI.initT);
                        resetObjects();
                        SKI.state = init;
                    }
                break;
            }

        }
        if (readButton()){          // Toggle zombie mode
            SKI.treeCode = (SKI.treeCode == SKI.zombieCode) ? 1 : SKI.zombieCode;
        }

        
    }
    return (EXIT_SUCCESS);
}

/*
    Prints out a frame to OLED
 */
void printFatFrame(FatFrame *frame, int offset){
    int maxPage = 3;
    
    OledSetCursor(0, 0 + offset);
    OledPutString(frame->row0);
    
    OledSetCursor(0, 1 + offset);
    OledPutString(frame->row1);
    
    OledSetCursor(0, 2 + offset);
    OledPutString(frame->row2);
    
    OledSetCursor(0, 3 + offset);
    OledPutString(frame->row3);
    

}
/*
    Initializes the OLED
 */
void beginMyOled(){
   DelayInit();
   OledInit();
}


void beginMyTimer3(){
    // 10ms Period.
    OpenTimer3(T3_ON         |          // timer 3 on
             T3_IDLE_CON    |           // don't stop on idle
             T3_SOURCE_INT  |           // Use internal clock
             T3_PS_1_16     |           // Prescale = 16
             T3_GATE_OFF,               // No gate control on timer
             6250);                      // counter register
}
void beginMyADC(){
    /*
     Start ADC on, with signed 32 bit int format, started by timer 3,
     samples continuously
    */

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


    EnableADC10();          // turn on converter              
}

void beginMyInterupts(){
    // ADC
    INTSetVectorPriority(INT_ADC_VECTOR, INT_PRIORITY_LEVEL_2);      // set priority 
    INTClearFlag(INT_AD1);                                           //clear interupt flag 
    INTEnable(INT_AD1, INT_ENABLED);                                 // Enable interupt
    
    // TIMER3
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_1);  // set priority 
    INTClearFlag(INT_T3);                                           //clear interupt flag
    INTEnable(INT_T3, INT_ENABLED);                                 // Enable interupt

    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);               //enable single/multi vector mode
    INTEnableInterrupts();                                           //enable interupts
}
/*
 Writes a glyph to absolute pixel position on OLED
 */
void SendCharToOled(int line, int pos, char str){

    OledMoveTo(pos, line);
    OledDrawGlyph(str);
    OledUpdate();

}
/*
    Set score during play time
 */
void setScore(int s){
    SKI.score += s;
    char buf[] = "   ";
    sprintf(buf,"%03d",SKI.score);
    SendCharToOled(3*8,13*8,buf[0]);
    SendCharToOled(3*8,14*8,buf[1]);
    SendCharToOled(3*8,15*8,buf[2]);
}
/*
    Increment level for game
 */
void setLevel(){
    SKI.level++;
    if (SKI.fallTime > 99) SKI.fallTime -= 50;
    char buf[] = "   ";
    sprintf(buf,"%03d",SKI.level);
    SendCharToOled(1*8,13*8,buf[0]);
    SendCharToOled(1*8,14*8,buf[1]);
    SendCharToOled(1*8,15*8,buf[2]);
}
/*
    Move skier left
 */
void moveLeft(){
    if (SKI.hPos <= 0) return;
    SendCharToOled(SKI.vPos, SKI.hPos, ' ');
    SKI.hPos--;
    SendCharToOled(SKI.vPos, SKI.hPos, SKI.skiCode);
}
/*
    Move skier right
 */
void moveRight(){
    if (SKI.hPos >= SKI.hPosMax) return;
    SendCharToOled(SKI.vPos, SKI.hPos, ' ');
    SKI.hPos++;
    SendCharToOled(SKI.vPos, SKI.hPos, SKI.skiCode);
}
/*
 Introduces a new gate or tree into playfield
 */
void throwRandomObject(){
    int num = rand() % SKI.objectsLength;
    int code = rand() % 10;
    
    if (code < 4) code = SKI.treeCode;
    else code = SKI.gateCode;
    
    throwObject((SKI.objects + num), code);
}

/*
 Brings an object into existence
 */
void throwObject(Object *obj, int code){
    if (obj->active) return;          // don't overwrite an existing object
    obj->vPos = 0;
    obj->hPos = (rand() % SKI.hPosMax);
    obj->CharCode = code;
    obj->active = 1;
    obj->dontScore=0;
   // SKI.activeObjects = SKI.activeObjects | sig;
}

/*
 Moves a specified object if it exists and handles any type of collision.
 */
void checkObject(Object *obj){
    if (SKI.state == gameover) return;
    int pos;
    if ((obj->vPos >= (16+obj->CharCode))){            //Check collision
        
        if (obj->CharCode == SKI.treeCode               //Check tree collision
           || obj->CharCode == (SKI.treeCode+SKI.codeOffset)){

            if (
                 (obj->hPos < (SKI.hPos+7))
                 && ((obj->hPos >= (SKI.hPos-7)))
               ){
                SKI.state = flash;
                return;
            }

        }else if (                                      //Check Gate collision
                 (obj->hPos < (SKI.hPos+4))
                 && ((obj->hPos >= (SKI.hPos-4)))
                ) {

            if (!obj->dontScore) setScore(1+SKI.level);
            obj->dontScore = 1;
            if (SKI.score > 0 && (SKI.score % SKI.gateThreshold)  == 0){
                setLevel();
            }
        }
        
    }
    
    if (obj->active){
        if (obj->vPos >= (SKI.vPosMax+4)){             // Kill obj
            obj->active = 0;
            SendCharToOled(obj->vPos, obj->hPos, ' ');
            if ((obj->CharCode == SKI.gateCode
                || obj->CharCode == (SKI.gateCode + SKI.codeOffset))
                && !obj->dontScore
               ){
                 setScore(-1);                      // Decrement Score
            }
            
        }else{                                      // Advance obj
            obj->CharCode
                  = (obj->CharCode == SKI.treeCode) ? (SKI.treeCode+SKI.codeOffset) :
                  (obj->CharCode == (SKI.treeCode+SKI.codeOffset)) ? SKI.treeCode :
                  (obj->CharCode == SKI.gateCode) ? (SKI.gateCode + SKI.codeOffset) :
                  SKI.gateCode;
            SendCharToOled(obj->vPos, obj->hPos, ' ');
            (*obj).vPos += 4;
            SendCharToOled(obj->vPos, obj->hPos, obj->CharCode);
        }
    };
    
}
/*
 Moves all existing objects in game
 */
void checkAllObjects(){
    int i;
    for (i=0; i < SKI.objectsLength; i++)
        if ((SKI.objects + i)->active) checkObject( (SKI.objects + i));
}
/*
    Reset stats so game is ready to start over
 */
void resetObjects(){
    int i;
    SKI.score = SKI.level = 0;
    SKI.gateThreshold = 3;
    SKI.fallTime = 500;
    SKI.isFlashing = 0;
    for (i=0; i < SKI.objectsLength; i++)
         (SKI.objects + i)->active = 0;
}
/*
    Returns a 1 or 0 if btn1 was pressed.
*/
static int ___lastPress_ = 0;
int readButton(){
    int cur = PORTG & BIT_6;
    if (cur == ___lastPress_) return 0;
    DelayMs(2);
    cur = PORTG & BIT_6;
    if (cur == ___lastPress_) return 0;
    ___lastPress_ = cur;
    if (cur) return 1;
    return 0;
}