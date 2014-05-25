/* 
 * File:   myheader.h
 * Author: Conor Patrick
 *
 * Created on March 30, 2014, 2:13 AM
 */

extern const BYTE rgbOledFont0[];
typedef enum {init=0, play, flash, gameover}__state__;
/*
    Object for tracking trees and gates
 */
typedef struct __object{
  int time;
  int vPos;
  int hPos;
  int CharCode;
  int active;
  int dontScore;
} Object;
/*
    Collection of stats for SKI game
 */
struct _SKI{
    int ADC;
    int time;
    int playFieldWidth;
    int score;
    int level;
    int hPos;
    int hPosMax;
    int vPos;
    int vPosMax;
    Object *objects;
    int objectsLength;
    int newGame;
    int gateThreshold;
    int isFlashing;
    int flashOn;
    
    int skiCode;
    int treeCode;
    int gateCode;
    int zombieCode;
    int codeOffset;
    
    int fallTime;
    int refreshTime;
    int throwTime;
    int initTime;
    int gameOverTime;
    int flashingTime;
    int flashTime;

    int throwT;
    int fall;
    int refresh;
    int initT;
    int gameOver;
    int flashing;
    int flash;

    __state__ state;
    
} SKI;
/*
    Object for pointing to multiple strings for printing an entire frame
 */
typedef struct __FatFrame{
  char *row0;
  char *row1;
  char *row2;
  char *row3;
} FatFrame;




