/*
    UART
        universal
        asyncronous
        reciever
        transmitter
    Data Frame
        the line idles high
        start bit - low for one bit time
        5-8 data bits - usually 8
        parity bit - even or odd. used for error detection
        1-2 stop bits (high)
    Rx & Tx agree in advance on stop bits, data bits, baud rate, parity
    
    parity error - when the parity is wrong
    framing error - when you don't get a stop bit
    
    full duplex- send and receive at the same time
    half duplex- can't do both at same time
    
    uses chip level voltage 0/5v, 0/3.3v, 0/2.5v
    
    RS-232 (history)
        defined 25 signal lines
        logic 0 was called a space @ +12v
        logic 1 was called a mark @ -12v
        
        connect teletypes to modems
            teletype - data terminal equipment (DTE)
                Tx - transmitted
                Rx - received
            modem - Data -circuit-terminating equipment (DCE)
                Tx - received
                Rx - transmitted
                
    baudrate = Fpb / (16*UxBRG + 1)
        you divide by 16 because clock runs 16x fast than baud rate
            so it samples 16 times per bit wow
        Fpb is the clock (10MHz)
        
        --__--------- --
        sampling stop bit in middle 75% is success (assumption)
            +/- 37.5% bit time
            9.5 bits * time per bit <= (0.375 - 1/16)*time per bit (sync error )
            delta t = (.375 - 1/16)/9.5 = 3.3%
    UxMODE - the mode its in
    UxSTA - status
    UxTXREG - transmit register
    UxRXREG - recieve register
    
    
*/

#define GetPGClock() 10000000


typedef enum{hey=1,hey1=2,hey3=3,hey4=4,wow} mytype;

int main(){
    
    /*TRISGCLR = 0xF000;
    
    UARTConfigure( UART1, UART_ENABLE_PINS_TX_RX_ONLY );
    
    UARTSetDataRate(UART1, GetPGClock(), 9600);
    
    UARTEnable( UART1, UART_ENABLE|UART_PERIPHERAL|UART_RX|UART_TX );*/
    mytype hi = hey;
    
    return 0;
}
















