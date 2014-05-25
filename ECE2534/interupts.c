/*

PORTG = PORTG | 0x1000; // set bit 12
    this could f up in an interupt
    
PORTGSET = 0x1000;
    this won't f up in an interupt
    
x = x + buttonPress();
    buttonPress is a routine that returns one if button has beeen pressed since
    last call.  Must be edge sensitive
    
Interupts
    provide a way for external devices to get CPU's attention
    
    execution is interupted and control is given to a
    interupt service routine (ISR).  At the conclusion of
    the ISR, control is transfered back to the interupted instuction
    
    associated with each interupt source
        vector number 0 to 63
            used to computer the start adress of ISR
        priority 0 to 7 (IPL - interupt priority level)
            0 is disabled and 7 is highest
        subpriority of 0 to 3
            extra priority for some systems that might be useful

    Interupt controller (peripheral)
        looks at all interupt requests and presents the interupt
        with highest priorty & sub priority to the CPU
            if the requested interupt priority is greater than CPU current
            prioriy, than control is transfered to the ISR at the vector addr
            otherwise interupt remains pending under CPU PL falls

*/


//debouncing
static int _lastPress = 0;
int buttonPress(){
    int cur = PORTG & 0x40;
    if (cur == _lastPress) return 0;
    delayMs(2);
    cur = PORTG & 0x40;
    if (cur == _lastPress) return 0;
    _lastPress = cur;
    if (cur) return 1;
    return 0;
}

int main(){
    
}