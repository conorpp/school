/*
disabled pin - tristate

reading/writing pins is done with registers
    TRISx - the tri-state register
        A--G
        Each bit corresponds to a pin e.g. bit 0 - pin 0
        set a bit to 1, output is tri state, pin acts as input
        set a bit to 0, out is not tristate, so pin is output
        Set to 1 as default
    PORTx
        A--G
        a write sends data to the data latch which drives outputs on pin
        a read gets data from the pin
        
        PORTA
            PORTASET - set corresponding bits to 1
            PORTACLR - set corresponding bits to 0
            PORTASINV - invert corresponding bits
        
*/


int main(){
    
    TRISGCLR = 0xF000;
    while(1){
        if (PORTG  & BIT_6)
            PORTGSET = 0xF000;
        else
            PORTGCLR = 0xF000;
    }
}

