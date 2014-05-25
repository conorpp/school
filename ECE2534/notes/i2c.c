
/*
I2C Bus
Inter Integrated Circuit Bus

Synchronous Serial
	Solves:
		Many chips on the same bus
		Multiple masters

SPI - 4 Wire Bus
	SDO
	SDI
	CS/SS
	SLCK

I2C - 2 Wire Bus
	SDA
	SCL

	Connect to I2C using open-drain gates
		open-drain gate is switch between
		high impedence or gnd
		
		A bus looks like a capacitor so
		it takes some current

	Remember Master& Slave
		controls clock
		sends START& STOP

	But with I2C there's only one line ...
		Sender& Receiver
		Receiver must ACK or NACK the Sender
	I2C
		Data must be constant whenever SCL is high
		START: SDA goes low when SCL is high
		1-7 bits to specify address (258 possible slaves)
		send read/write commands
	START -> Address -> R/W Bit -> data -> ACK/NACK response + data
	To change W/R in session, the START+Address must be started over.
		
	Multiple masters on same bus
		is possible
		arbitration is who controls the bus
			can only be one
		clock control	
			only one master can send clock
			the 0 that gets sent always wins

	ACK - drive high
	NACK - drive low
*/

int main(){
	
	I2CBusIsIdle(I2C_MODULE);
	
	//Asset start
	I2CStart(I2C_MODULE); // check if START finished
	I2CSendByte(I2C_MODULE, byte);
	
	I2CTransmissionHasCompleted(); // check if finished

	I2CByteWasAcknowledged(); // check if ack'd

	/* now as reciever  */	
	I2CSendByte(...)

	I2CRevieverEnable(...);
	I2CRecievedDataIsAvailable(...);
	I2CGetByte(...);
	I2CAckknowledgeByte(...);

	I2CAcknowledgeHasCompleted(...);

	// send STOP
	I2CStop(I2C_MODULE);

    return 0;
}
typedef int bool;
typedef int BYTE;


void sendStart(I2C_MODULE i2c){
        while( !!! I2CBusIsIdle(i2c));
        I2CStart(i2c);
        I2CGetStatus(i2c);
        while( !!! (I2CGetStatus(i2c) & I2C_START) );
}


BYTE readByte(I2C_MODULE i2c){
    I2CReceiverEnable(i2c, TRUE);
    while ( !!! I2CReceivedDataIsAvailable(i2c));
    BYTE t;
    t = I2CGetByte(i2c);
    I2CAcknowledgeByte(i2c, FALSE);
    while( !!! I2CAcknowledgeHasCompleted(i2c));
    return t;
}


bool writeByte(I2C_MODULE i2c, BYTE b); // SEND ACK AND CHECK

BYTE readByte(I2C_MODULE i2c);

void sendStop(I2C_MODULE i2c);

//  void _restart_bus_(I2C_MODULE i2c); // ?


BYTE accelReadReg(I2C_MODULE i2c, int reg){
	sendStart(i2c);
	int notAcked;
	// accel slave addr is 0c1d
	// write addr and set write bit (0)
	notAcked = sendByte(i2c, (0x1d << 1) + 0);
	if (notAcked){
		while(1);	//halt if no slave	
	}
	notAcked = sendByte(i2c, reg);
	if (notAcked){
		while(1);
	}

	// now turn bus around
	restartBus(i2c);
 	// and read (1)
	notAcked = sendByte(i2c, (0x1d << 1) + 1);
	if (notAcked){
		while(1);	//halt if no slave	
	}

	int temp = readByte(i2c);
	sendStop(i2c);
	return temp;

}

bool accelWriteReg(I2C_MODULE, int reg, int *data);

BYTE* accelReadData();


























