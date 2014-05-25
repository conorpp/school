/*
Communication	
    ->parallel
	->async (UART) 
            -long distance
    ->serial
        ->SPI
            -> master/slave
		- the master drives the clock. there can only be one clock.
            -ancient protocol like UART
            ->wires
                -slave select SS
                -SDO
                -SDI
                -SCK
            -> Two really important bits
                ->CKP - clock polarity when idle
                    - idle clock level
                    - 0 = low
		    - 1 = high
		->CKE - clock edge when data changes
		    - 0 = data changes on idle to active  clock edge
		    - 1 = changes on active to idle edge
            - timing diagrams are important
        -> uses three registers on pic to talk
            - SPIxCON
            - SPIxSTAT
            -> SPIxBRG
                Fpb / 2(BRG+1)
            -> SPIxBUF
                - is actually separate writing/reading register stacks.
	    
	    -MSSEN register - selects chip as slave or master
	    -SPIBUSY register - indicates if SPI is busy or not
	    -SPIROV - receive overflow. 
	    -SPIUR - under run error if slave isn't feeding data to master fast enough
	    
	-> PLIB - see deh codez 
	-> data is sent active low if the docs specify a character like #, *, _, | 	    
    
    ->Master/Slave
        -MOSI - master out slave in
        -MISO - master in slave out
        
        -Master controls the clock
    
        -Clock typically doesn't run unless data is being sent
        
	
    -DOUT (Data out) is nearly always from point of view from master

OLED
    32 rows od 128 pixels
	pin 0, pin 1, pin 2 ... pin 128 for each row
	dat refresh rate

    Talk to it via a SPI bus





*/

int main(){
	SpiChnOpen
	SpiChnDataReady	// check if data is there
	SpiChanGetC	// blocks until channel is available.
	SpiChanPutC	// blocks while channel is full.
	
	return 0;
}




