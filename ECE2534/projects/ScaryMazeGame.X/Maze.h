/* 
 * File:   Maze.h
 * Author: Conor Patrick
 *
 * This header includes utilities for the maze game including:
 *      - Interfaces for reading the ADXL345 with SPI and I2C
 *      - Functions for initializating the OLED and the Maze
 *
 * Created on April 24, 2014, 11:37 PM
 */

#define Accel_SPI 1
#define Accel_I2C 0
#define enableAccel
/*
    The typedef for accelerometer data object
 */

typedef signed short int AXIS_DATA;
typedef struct {
    /* function pointers */
    int (*_read_reg)(const struct Accelerometer *, int);    // register read pointer
    int (*write)(const struct Accelerometer *, int, int);   // register write pointer
    int (*read)(struct Accelerometer *);                    // Axis read pointer
    /* Axis data */
    AXIS_DATA axis[3];
    /* I2C/SPI configuration */
    int channel, readDevice, writeDevice, ID, type;
} Accelerometer;


void drawMaze();
void initOled();
void init_I2C_Bus(I2C_MODULE i2c);
void init_SPI_Bus(SpiChannel chn);

void OledPrintLine(char *str, int column);

int _SPI_readReg(const Accelerometer *A, int addr);
void _SPI_writeReg(const Accelerometer *A, int addr, int val);
int _I2C_readReg(const Accelerometer *A, int reg);
void _I2C_writeReg(const Accelerometer *A, int reg, int val);
int _readAccel(Accelerometer *A);

/*
    Initializes a data object for the accelerometer
    and sets function pointers depending on I2C or SPI
 */
void constructAccel(Accelerometer *A, int SPI_or_I2C){
    int i;
    for (i = 0; i<3; i++)           // Set axis data to 0
        A->axis[i] = 0;

    if ( SPI_or_I2C == Accel_SPI ){ // Setup SPI
        TRISFCLR = BIT_12;
        init_SPI_Bus(SPI_CHANNEL4);
        A->channel = SPI_CHANNEL4;  // Set channel to SPI
        
        /* Point to SPI read/write functions */
        A->_read_reg = _SPI_readReg;
        A->write = _SPI_writeReg;
        
    }else if ( SPI_or_I2C == Accel_I2C ){      // I2C
        init_I2C_Bus(I2C2);         // setup I2C
        A->channel = I2C2;
        A->readDevice = 0x3b;       // Device address 
        A->writeDevice = 0x3a;

        /* Point to I2C read/write functions */
        A->_read_reg = _I2C_readReg;
        A->write = _I2C_writeReg;
    }
    A->read = _readAccel;           // Point to high level read function

    A->write(A, 0x2d, 0x8);         // put in measure mode
    A->write(A, 0x2c, 0x0A);        // Sample at a 100 Hz
    A->type = SPI_or_I2C;
    A->ID = A->_read_reg(A, 0x0);   // read ID
}


/*
    Initializes the OLED
 */
void initOled(){
   DelayInit();
   OledInit();
}

/*
    Displays a message on the OLED
 */
void OledPrintLine(char *str, int column){
    if (!*str) return;
    OledSetCursor(0, column);
    OledPutString(str);
}

/*
    Initializes the SPI bus
 */
void init_SPI_Bus(SpiChannel chn){
    SpiChnOpen(chn,
         /* master mode, idle clock high, transmit from idle to active */
            SPI_OPEN_MSTEN | SPI_OPEN_CKP_HIGH |
         /* 8 bits, turn spi on, enhanced buffer mode */
            SPI_OPEN_MODE8  |SPI_OPEN_ENHBUF,
        /*set bitrate to 10MHz/20 = 0.5 MHz */
            20
    );
}

/*
    Initializes the I2C bus
 */
void init_I2C_Bus(I2C_MODULE i2c){
    I2CConfigure(i2c, I2C_ENABLE_HIGH_SPEED);
    I2CSetFrequency(i2c,    
                    10*1000*1000,// Freq of cerebot clock
                    400*1000);   //400 kHz is fastest the ADXL345 i2c can go
    I2CEnable(i2c, 1);           // turn on I2C channel
}


/*
    Sends a byte on an i2c channel 
 */
void _I2C_sendByte(I2C_MODULE i2c, BYTE byte){
    I2CSendByte(i2c, byte);                     // Send and wait for finish
    while( ! I2CTransmissionHasCompleted(i2c));
    while( ! (I2CByteWasAcknowledged(i2c)));    // wait for ack
}
/*
    Sends a start to begin i2c transaction (with no strings attached)
 */
void _I2C_sendStart(I2C_MODULE i2c){
        while( ! I2CBusIsIdle(i2c));
        DelayMs(2);                                 // timing for ADXL345
        I2CStart(i2c);
        while( ! (I2CGetStatus(i2c) & I2C_START) ); // Wait for start condition
        I2CClearStatus(i2c, I2C_START);
}


/*
    Sends a start inside i2c transaction
 */
void _I2C_repeatStart(I2C_MODULE I2C){
    I2CRepeatStart(I2C);
    while( ! (I2CGetStatus(I2C) & I2C_START) );
    I2CClearStatus(I2C, I2C_START);
}

/*
    Reads a byte from I2C RX buffer
 */
int _I2C_readByte(I2C_MODULE i2c){

    I2CReceiverEnable(i2c, TRUE);               // Allow recieving data
    while ( ! (I2CReceivedDataIsAvailable(i2c)));

    int t;
    t = I2CGetByte(i2c);                        // Get byte

    I2CReceiverEnable(i2c, FALSE);              // Disallow receive data
    return t;
}
#ifdef enableAccel
/*
    Writes a value to a register with I2C
 */
void _I2C_writeReg(const Accelerometer *A, int reg, int val){

    _I2C_sendStart(A->channel);                 // start transaction
    _I2C_sendByte(A->channel, A->writeDevice);  // select device by address 
    _I2C_sendByte(A->channel, reg);             // accel device register
    _I2C_sendByte(A->channel, val);             // value
    I2CStop(A->channel);
}

/*
    Reads a register value with I2C
 */
int _I2C_readReg(const Accelerometer *A, int reg){
    _I2C_sendStart(A->channel);                     // start transaction
    _I2C_sendByte(A->channel, A->writeDevice);      // Select device by address
    _I2C_sendByte(A->channel, reg);                 // Select register to read from
    _I2C_repeatStart(A->channel);                   // Turn bus aroundd
    _I2C_sendByte(A->channel, A->readDevice);       // Select device with read bit set
    int d = _I2C_readByte(A->channel);              // Grab data sent back
    I2CAcknowledgeByte(A->channel, 0);              // Send nack
    while(!I2CAcknowledgeHasCompleted(A->channel));
    I2CStop(A->channel);                            // end transaction
    return d;
}

/*
    Writes a value to a register with SPI
 */
void _SPI_writeReg(const Accelerometer *A, int addr, int val){
    PORTFCLR = BIT_12;                  // Select the slave
    SpiChnPutC(A->channel,((addr)));    // Write register address with write bit
    SpiChnGetC(A->channel);             // clear junk data in Rx buffer
    SpiChnPutC(A->channel,( val ));     // write value to register
    SpiChnGetC(A->channel);             // clear junk data in Rx buffer
    while(SpiChnIsBusy(A->channel));
    PORTFSET = BIT_12;                  // Deselect the slave
}
/*
    Reads a register value with SPI
 */
int _SPI_readReg(const Accelerometer *A, int addr){

    PORTFCLR = BIT_12;                          // Select the slave

    SpiChnPutC(A->channel,( (BIT_7) | addr));   // Send device address with read bit.
    SpiChnGetC(A->channel);                     // Clear the buffer of junk data
    SpiChnPutC(A->channel,( 0xff));             // Enable clock to get new data
    while(SpiChnIsBusy(A->channel));

    PORTFSET = BIT_12;                          // Deselect slave
    return SpiChnGetC(A->channel);
}
#endif

/* Draws the maze.
** Code credit to Dr. Baumann
**/
void drawMaze(){
    OledClearBuffer();
    const int grid[]={27,30,44,47,70,73,107,110,127,
                      50,53,79,82,90,93,127,
                      20,23,67,70,106,109,127};
    OledMoveTo(0,0);
    OledDrawRect(127,31);
    int xc=0;
    int yc=7;
    int ind=0;
    OledMoveTo(xc,yc);
    while(1) {
        OledLineTo(grid[ind],yc);
            if (grid[ind++]==127) {
                yc+=8;
                OledMoveTo(0,yc);
                if(yc>30) break; //we're done
            }
            else OledMoveTo(grid[ind++],yc);
    }
    OledMoveTo(64,0);
    OledLineTo(64,23);
    OledMoveTo(40,7);
    OledLineTo(40,15);
    OledMoveTo(85,7);
    OledLineTo(85,23);
    OledMoveTo(96,23);
    OledLineTo(96,31);
    OledUpdate();
}