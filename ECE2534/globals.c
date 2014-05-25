

#include <stdio.h>

typedef struct {
    int secs;
    int mins;
    int hrs;
} _ISRStruct;

//global
volatile _ISRStruct time;
_ISRStruct *p;

p = &time;

int main(){
    time.secs++;
    printf("secs: %d\n", time.secs);
}