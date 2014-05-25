/*

OpenTimerX
CloseTimerX
ConfigIntTimerX
Disable
Enable
ReadTime
Write
ReadPerif
Write
configTimerX

*/
// PRAGMA STUFF

#include <stdio.h>

enum {init, waitStart, playGame} mode;


void myFunc(){
    printf("hello\n");
}

void executeCallback(void (*callback)(void)){
    
    callback();
}

int main(){
    mode= init;
    mode = playGame;
    switch(mode){
        case init:
            printf("INIT\n");
        break;
        case waitStart:
            printf("WAITSTART\n");
        break;
        case playGame:
            printf("PLAYGAME\n");
        break;
    }
    executeCallback(myFunc);
    return 0;
}










