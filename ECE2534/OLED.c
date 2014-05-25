/*
__________________
|page|       | 
| 0  | 8x128 |
| 1  | 8x128 |
| 2  | 8x128 |
..................

turn on pixel 20,57
page 2 (bits 16-23) for row
col 57
0x10 - not sure

every character fits into an 8x8 square
read bottom to top:

40 , 7C, 4A, 09, 4A, 7C, 40, 00
             x   
         x       x
     x               x
     x   x    x   x  x
     x               x
     x               x
x        x       x       x


OLED commands:

0x22
	begin line
	end line
	0x0-0x0f set the LSN of col addr
	0x10-0x1f set the MSN of the col addr
other cmds ... 

PORTF - SPI4
PORTE - SPI3

*/

// forward declaration for something defined later in compilation
extern const BYTE rgbOledFont0[];

void SendCharToOled(int line, int col, char letter);

#define main herewego
int herewego(){
	
	PORTDCLR = BIT_9 | BIT_1;

	SpiChnPutC(SPI_CHANNEL1, 0xA5);		// all LEDs on
		
	// PORTDSET = BIT_9;			// LEDS F'd

	while(SpiChnIsBusy(SPI_CHANNEL1));


	// PORTDSET = BIT_9;			// works

	int line = 0;
	int col = 0;
	char letter = 'a';
	SendCharToOled(line, col, letter);

	

	while(1);


	return 0;
}


/*
    line: 0-3 line to start writing
    pos: 0-15 character position
    string: ascii string to send
    NOTE:
	must set OLEDBRG
 
*/
void SendCharToOled(int line, int pos, char *str){
    PORTDCLR = BIT_9  |  BIT_1;
    
    // Set the page (row) 
    // set horizontal mode
    SpiChnPutC(SPI_CHANNEL1, 0x22 );
    SpiChnPutC(SPI_CHANNEL1, (line & 0x3) );
    
    // Set lower and upper column nibbles
    // Multiply by 8 because a char is 8 pixels wide
    SpiChnPutC(SPI_CHANNEL1, ((pos << 3) & 0xF) )
    SpiChnPutC(SPI_CHANNEL1, ((pos >> 1) & 0x1F) )

    // wait for SPI to finish sending
    while(SpiChnIsBusy(SPI_CHANNEL1));

    // Set SPI to data mode
    PORTDSET = BIT_1;
    int i;
    while(*str){
        for(i = 0; i < 8; i++){
	    SpiChnPutC(SPI_CHANNEL1, rgbOledFont[ (*str - 0x20)*8 + i ]);
	    // position will auto inc
    	}
	str++;
    }
    
    while(SpiChnIsBusy(SPI_CHANNEL1));
    PORTDSET = BIT_9;
    
}






















