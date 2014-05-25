/* 
 * File:   main.c
 * Author: Conor Patrick
 *
 * This code meets the requirements for the assignment.
 * I wrote a header file "Maze.h"
 * 
 * Created on April 28, 2014, 3:56 PM
 */
#define _PLIB_DISABLE_LEGACY
#include <plib.h>
#include <stdlib.h>
#include "PmodOLED.h"
#include "OledChar.h"
#include "OledGrph.h"
#include "delay.h"
#include "Maze.h"

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


// Typedef for maze player
typedef struct {
    int x, y;
    int dx, dy;
    int Vx, Vy;
    int width, height;
} MazePlayer;

/*******************************************/
/****************GLOBAL VARIABLES***********/

unsigned int TIME_MS;                       /* Timer */
Accelerometer *_Accel_ISR_Handle = NULL;   /* Give Timer ISR access to
                                              Accelerometer read function. */
/*******************************************/

/*
    Interupt Handler for Timer2
*/
void __ISR(_TIMER_2_VECTOR, ipl2) TIMER3_HANDLER( void )
{
    if( INTGetFlag(INT_T2) )            // Verify source of interrupt
   {
      TIME_MS++;

      if ( !(TIME_MS % 10) ){           // Read Accelerometer every 10 ms
          
        if (_Accel_ISR_Handle != NULL)
            /* Data will be read if data is available. */
            _Accel_ISR_Handle->read(_Accel_ISR_Handle);
      }

      INTClearFlag(INT_T2);             // Acknowledge interrupt
   }
}
/**************************************/
/**********Forward Declarations***********/
int readButton(int btn, int *lastPress);
int readButton3(int *lastPress);

void clearChar(int x, int y);
void setChar(int x, int y);
void _drawChar(int x, int y);


void movePlayer(int *x, int *y, int dx, int dy);


void resetTime(int *time){*time = TIME_MS;}

int pixelAt(int x, int y);

void bounceX(MazePlayer *M);
void bounceY(MazePlayer *M);
/*************************************************/
/*************************************************/

int main(){
    /*** Initializations ****/
    
    // Peripherals
        startTimer2();
        initOled();
        DDPCONbits.JTAGEN = 0;  // Must disable JTAG to use btn3
        TRISASET = BIT_0;       // btn3
        
    // Accelerometer instance
        char buf[17] = "                ";
        Accelerometer Accel;

        
    // Maze player instance and settings
        MazePlayer dude;
        dude.dx = 0, dude.dy = 0;
        dude.Vx = 0, dude.Vy = 0;
        dude.width = 2, dude.height = 2;

        enum {init, pick, startMaze, maze, score, reset} state;
        state = reset;
        
        int VMax = 13200;       // Maximum speed in pixels/s
        int gameSpeed = 330;    // Speed scale for game play
        int friction = 12;      // friction equation = (friction-1)/friction
        int xTol = 500;         // X and Y velocity tolerances for not moving.
        int yTol = 100;
        int btn1 = BIT_6;
        int btn1LastPress = 0;  // Buttons
        int btn2 = BIT_7;
        int btn2LastPress = 0;
        int btn3LastPress = 0;
        
    // time stamps
        /* read position every 10ms */
        int dt = 10, readStampMS = TIME_MS;
        
        int Xstamp = 30, XstampMS = TIME_MS;
        int Ystamp = 30, YstampMS = TIME_MS;
        int updateTime = TIME_MS;
        int gameT;

        int sensitivity = 1;

        int Ax, Ay, Vx, Vy, VxMagnitude, VyMagnitude;

    /***************************/
        
    while(1){
        switch(state){
            /* Turn off devices and print messages before starting.
             */
            case reset:
                /* Close any open interfaces */
                if (Accel.type == Accel_SPI)
                    SpiChnClose(Accel.channel);
                else if (Accel.type == Accel_I2C)
                    I2CEnable(Accel.channel, 0);

                dude.x = 1, dude.y = 1;
                OledClear();
                OledPrintLine("C.P.P.",0);
                OledPrintLine("Cerebot    ",1);
                OledPrintLine("Maze Puzzle    ",2);
                state = init;
            case init:
                /* Print prompt for interface */
                if (readButton(btn1, &btn1LastPress)){
                    OledClear();
                    OledPrintLine("Pick interface",0);
                    OledPrintLine("Button 1: SPI",1);
                    OledPrintLine("Button 2: I2C",2);
                    state = pick;
                }
            break;
            case pick:
                /* Listen for SPI or I2C and set up accordingly */
                if (readButton(btn1, &btn1LastPress)){
                    constructAccel(&Accel, Accel_SPI);
                    state = startMaze;
                }
                else if(readButton(btn2, &btn2LastPress)){
                    constructAccel(&Accel, Accel_I2C);
                    state = startMaze;
                }
                
            break;
            case startMaze:
                    /* Initialize for the maze */
                    movePlayer(&dude.x, &dude.y, 0, 0);
                    drawMaze();
                    gameT = TIME_MS;
                    state = maze;
                    _Accel_ISR_Handle = &Accel;
            break;
            case maze:
                /* Play the maze */
                if (TIME_MS - dt > readStampMS){
            
                    resetTime(&readStampMS);
                    
                    /* Determine acceleration in pixels/s^2
                        Data comes in mapped from -100 to 100, where 100 is 1g
                     *  1g = 9.8 m/s^2 = 52678 pixels/s^2
                     *
                     *  1 pixel/s^2 = (1g)*52678/100
                     *
                     */
                    Ax = (Accel.axis[1]/sensitivity)*sensitivity,
                    Ax = (Accel.axis[1] * 52678) / 100;


                    Ay = (Accel.axis[0]/sensitivity)*sensitivity,
                    Ay = (Accel.axis[0] * 52678) / 100;

                    /*  Velocity is Vo + V*A
                     *      Convert ms to s
                     */
                    Vx = (Ax * dt)/1000;
                    Vy = (Ay * dt)/1000;
                    dude.Vx += Vx;
                    dude.Vy += Vy;

                }
                /*
                 Calculate velocity magnitudes to make
                 timestamps functions of velocity.
                */
                VxMagnitude = dude.Vx < 0 ? dude.Vx*-1 : dude.Vx;
                VyMagnitude = dude.Vy < 0 ? dude.Vy*-1 : dude.Vy;

                /* move player in X direction */
                if (TIME_MS > XstampMS + ((VMax - VxMagnitude)/gameSpeed)){
                    resetTime(&XstampMS);

                    /* set direction */
                    dude.dx = dude.Vx < 0 ? 1 : -1;
                    if (dude.Vx > -1*xTol && dude.Vx < xTol ) dude.dx = 0;
                    
                    /* flip direction if a limit is hit */
                    bounceX(&dude);

                    /* Move player X component */
                    movePlayer(&dude.x, &dude.y, dude.dx, 0);

                    /* Apply friction */
                    dude.Vx = (dude.Vx * (friction-1))/friction;    
                }
                /* move player in Y direction*/
                if (TIME_MS > YstampMS + ((VMax - VyMagnitude)/gameSpeed)){
                    resetTime(&YstampMS);
                    
                    /* set direction */
                    dude.dy = dude.Vy < 0 ? 1 : -1;
                    if (dude.Vy > -1*yTol && dude.Vy < yTol ) dude.dy = 0;

                    /* flip direction if a limit is hit */
                    bounceY(&dude);

                    /* Move player  Y component */
                    movePlayer(&dude.x, &dude.y, 0, dude.dy);

                    /* Apply friction */
                    dude.Vy = (dude.Vy * (friction-1))/friction;    
                }
                /* Check to see if player has reached the end */
                if (dude.x > 120 && dude.y > 24){
                    gameT = TIME_MS - gameT;

                    OledClear();
                    /* Print out time in maze */
                    sprintf(buf, "Time: %d.%d sec", gameT/1000,
                                                    gameT - (gameT/1000)*1000);
                    OledPrintLine(buf,3);
                    OledPrintLine("Game Over",0);
                    state = score;
                }
            break;
            case score:
                /* Do nothing.  Game over until reset. */
            break;
        }
        /* Restart Game if button 3 is pressed */
        if (readButton3(&btn3LastPress)){
            state = reset;
        }

    }

    return EXIT_SUCCESS;
}

/*
 Maps a value to a particular range
*/
int map(int v1, int v2, int r1, int r2, int val){
    return (((val*1000/(v2-v1)) * (r2-r1)))/1000;
}

/*
     Read data in accelerometer if data is available.
     Return indicator if new data came in.
*/
int _readAccel(Accelerometer *A){
    /* don't bother reading if data is not ready. */
    if ( !(A->_read_reg(A, 0x30) & BIT_7) ) return 0;

    unsigned char i;
    AXIS_DATA n1,n2;

    /* Read value for each axis into 16 bit int (short int)
     */
    for (i=0; i<3; i++){
        /* Read value into 16 bit integer (short int) */
        n1 = A->_read_reg(A, 0x32 +i*2 );
        n2 = A->_read_reg(A, 0x33 +i*2 );
        A->axis[i] = n1 | (n2 << 8);
        /* convert to   100*g = 100*52678 pixels/s^2
         */
        A->axis[i] = map(0, 0xff, 0, 100, A->axis[i]);
    }

    return 1;
};

/*
 Start the timer
 */
void startTimer2(){
    // 10ms Period.
    OpenTimer2(T2_ON         |          // timer 3 on
             T2_IDLE_CON    |           // don't stop on idle
             T2_SOURCE_INT  |           // Use internal clock
             T2_PS_1_16     |           // Prescale = 16
             T2_GATE_OFF,               // No gate control on timer
             625);                      // every 1 ms
    // TIMER2 interupt
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_1);  // set priority
    INTClearFlag(INT_T2);                                           //clear interupt flag
    INTEnable(INT_T2, INT_ENABLED);                                 // Enable interupt

    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);               //enable single/multi vector mode
    INTEnableInterrupts();
}
/*
 Can specify btn1 or btn2 and it will return
 * whether or not it's triggered.
 */
int readButton(int btn, int *lastPress){
    int cur = PORTG & btn;
    if (cur == *lastPress) return 0;
    DelayMs(2);
    cur = PORTG & btn;
    if (cur == *lastPress) return 0;
    *lastPress = cur;
    if (cur) return 1;
    return 0;
}
/*
 Returns 1 if btn3 is triggered.
 */
int readButton3(int *lastPress){
    int cur = PORTA & 1;
    if (cur == *lastPress) return 0;
    DelayMs(2);
    cur = PORTA & 1;
    if (cur == *lastPress) return 0;
    *lastPress = cur;
    if (cur) return 1;
    return 0;
}
/*
  Subroutine: Clears the player at the specified position
 */
void clearChar(int x, int y){
    OledSetDrawMode(3);     // Set XOR mode to clear
    _drawChar( x, y);
    OledSetDrawMode(0);     // Return to set mode
}
/*
 Subroutine: Draws player in specified position
 */
void setChar(int x, int y){
    _drawChar( x, y);
}
/*
 Subroutine: Draws or clears player in specified position
 */
void _drawChar(int x, int y){

    OledMoveTo(x, y);
    OledDrawPixel();

    OledMoveTo(x+1, y);
    OledDrawPixel();

    OledMoveTo(x, y+1);
    OledDrawPixel();

    OledMoveTo(x+1, y+1);
    OledDrawPixel();

}

/*
 Moves player by specified position by a specified delta
 */
void movePlayer(int *x, int *y, int dx, int dy){
    clearChar( *x,  *y);
    *x += dx;
    *y += dy;

    setChar(*x, *y);
    if (*x > 126 || *y > 31){
        clearChar( *x,  *y);
    }
    OledUpdate();
}

/*
 Returns the value of a pixel
 */
int pixelAt(int x, int y){
    OledMoveTo(x, y);
    return OledGetPixel();
}

/*
 * Check borders for X directions and switch direction
 * if needed
 */
void bounceX(MazePlayer *M){

    /* Check screen limit */
    if (M->x < 1){
        M->dx = 1;
        if (M->Vx > 0) M->Vx *= -1;
    }else if (M->x > 125){
        M->dx = -1;
        if (M->Vx < 0) M->Vx *= -1;
    }

    /* Check (-, 0) border */
    if (pixelAt(M->x - 1, M->y)){
        M->dx = 1;
        if (M->Vx > 0) M->Vx *= -1;
    }else
    /* Check  (+, 0) border*/
    if (pixelAt(M->x + M->width, M->y)){
        M->dx = -1;
        if (M->Vx < 0) M->Vx *= -1;
    }
    /* Check +/- X */
    if (pixelAt(M->x + M->width, M->y) && pixelAt(M->x -1, M->y)){
        M->dx = 0;
        M->Vx = 0;
    }else
    /* Check +/- X */
    if (pixelAt(M->x + M->width, M->y+1) && pixelAt(M->x -1, M->y+1)){
        M->dx = 0;
        M->Vx = 0;
    }
}
/*
 * Check borders for Y directions and switch direction
 * if needed
 */
void bounceY(MazePlayer *M){

    if (M->y < 1){          // check screen limit
        M->dy = 1;
        if (M->Vy > 0) M->Vy *= -1;
    }else if (M->y > 29){
        M->dy = -1;
        if (M->Vy < 0) M->Vy *= -1;
    }

    /* Check (0, -) border */
    if (pixelAt(M->x, M->y -1)){
        M->dy = 1;
        if (M->Vy > 0) M->Vy *= -1;
    }else
    /* Check  (0, +) border*/
    if (pixelAt(M->x , M->y+ M->height)){
        M->dy = -1;
        if (M->Vy < 0) M->Vy *= -1;
    }
}