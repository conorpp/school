/************************************************************************/
/*																		*/
/*	oled.c	--	Graphics Driver Library for OLED Display				*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): Created											*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <p32xxxx.h>
#include <plib.h>

#include "PmodOLED.h"
#include "OledChar.h"
#include "OledGrph.h"

#include "delay.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern BYTE		rgbOledFont0[];
extern BYTE		rgbOledFontUser[];
extern BYTE		rgbFillPat[];

extern int		xchOledMax;
extern int		ychOledMax;

/* Coordinates of current pixel location on the display. The origin
** is at the upper left of the display. X increases to the right
** and y increases going down.
*/
int		xcoOledCur;
int		ycoOledCur;

BYTE *	pbOledCur;			//address of byte corresponding to current location
int		bnOledCur;			//bit number of bit corresponding to current location
BYTE	clrOledCur;			//drawing color to use
BYTE *	pbOledPatCur;		//current fill pattern
int		fOledCharUpdate;

int		dxcoOledFontCur;
int		dycoOledFontCur;

BYTE *	pbOledFontCur;
BYTE *	pbOledFontUser;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* This array is the offscreen frame buffer used for rendering.
** It isn't possible to read back frome the OLED display device,
** so display data is rendered into this offscreen buffer and then
** copied to the display.
*/
BYTE	rgbOledBmp[cbOledDispMax];

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void	OledHostInit();
void	OledDevInit();
void	OledDvrInit();
BYTE	Spi1PutByte(BYTE bVal);
void	OledPutBuffer(int cb, BYTE * rgbTx);

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	OledInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display subsystem.
*/

void
OledInit()
	{

	/* Init the PIC32 peripherals used to talk to the display.
	*/
	OledHostInit();

	/* Init the memory variables used to control access to the
	** display.
	*/
	OledDvrInit();

	/* Init the OLED display hardware.
	*/
	OledDevInit();

	/* Clear the display.
	*/
	OledClear();

}

/* ------------------------------------------------------------ */
/***	OledHostInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Perform PIC32 device initialization to prepare for use
**		of the OLED display.
*/

void
OledHostInit()
	{
//	unsigned int	tcfg;                   // ALA unused

	/* Initialize SPI port 1.
	*/
	SPI1CON = 0;
	SPI1BRG = 15;				//8Mhz, with 80Mhz PB clock
	SPI1STATbits.SPIROV = 0;
	SPI1CONbits.CKP = 1;
	SPI1CONbits.MSTEN = 1;
	SPI1CONbits.ON = 1;

	/* Make power control pins be outputs with the supplies off
	*/
	PORTSetBits(prtVddCtrl, bitVddCtrl);
	PORTSetBits(prtVbatCtrl, bitVbatCtrl);
	PORTSetPinsDigitalOut(prtVddCtrl, bitVddCtrl);		//VDD power control (1=off)
	PORTSetPinsDigitalOut(prtVbatCtrl, bitVbatCtrl);	//VBAT power control (1=off)

	/* Make the Data/Command select, Reset, and SPI CS pins be outputs.
	*/
	PORTSetBits(prtDataCmd, bitDataCmd);
	PORTSetPinsDigitalOut(prtDataCmd, bitDataCmd);		//Data/Command# select
	PORTSetBits(prtReset, bitReset);
	PORTSetPinsDigitalOut(prtReset, bitReset);
	PORTSetBits(prtSelect, bitSelect);
	PORTSetPinsDigitalOut(prtSelect, bitSelect);

}

/* ------------------------------------------------------------ */
/***	OledDvrInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED software system
*/

void
OledDvrInit()
	{
	int		ib;

	/* Init the parameters for the default font
	*/
	dxcoOledFontCur = cbOledChar;
	dycoOledFontCur = 8;
	pbOledFontCur = rgbOledFont0;
	pbOledFontUser = rgbOledFontUser;

	for (ib = 0; ib < cbOledFontUser; ib++) {
		rgbOledFontUser[ib] = 0;
	}

	xchOledMax = ccolOledMax / dxcoOledFontCur;
	ychOledMax = crowOledMax / dycoOledFontCur;

	/* Set the default character cursor position.
	*/
	OledSetCursor(0, 0);

	/* Set the default foreground draw color and fill pattern
	*/
	clrOledCur = 0x01;
	pbOledPatCur = rgbFillPat;
	OledSetDrawMode(modOledSet);

	/* Default the character routines to automaticall
	** update the display.
	*/
	fOledCharUpdate = 1;

}

/* ------------------------------------------------------------ */
/***	OledDevInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display controller and turn the display on.
*/

void
OledDevInit()
	{

	/* We're going to be sending commands, so clear the Data/Cmd bit
	*/
	PORTClearBits(prtDataCmd, bitDataCmd);

	/* Start by turning VDD on and wait a while for the power to come up.
	*/
	PORTClearBits(prtVddCtrl, bitVddCtrl);
	DelayMs(1);

	/* Display off command
	*/
	Spi1PutByte(0xAE);

	/* Bring Reset low and then high
	*/
	PORTClearBits(prtReset, bitReset);
	DelayMs(1);
	PORTSetBits(prtReset, bitReset);

	/* Send the Set Charge Pump and Set Pre-Charge Period commands
	*/
	Spi1PutByte(0x8D);
	Spi1PutByte(0x14);

	Spi1PutByte(0xD9);
	Spi1PutByte(0xF1);

	/* Turn on VCC and wait 100ms
	*/
	PORTClearBits(prtVbatCtrl, bitVbatCtrl);
	DelayMs(100);

	/* Send the commands to invert the display.
	*/
	Spi1PutByte(0xA1);			//remap columns
	Spi1PutByte(0xC8);			//remap the rows

	/* Send the commands to select sequential COM configuration
	*/
	Spi1PutByte(0xDA);			//set COM configuration command
	Spi1PutByte(0x20);			//sequential COM, left/right remap enabled

	/* Send Display On command
	*/
	Spi1PutByte(0xAF);

}

/* ------------------------------------------------------------ */
/***	OledClear
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display. This clears the memory buffer and then
**		updates the display.
*/

void
OledClear()
	{

	OledClearBuffer();
	OledUpdate();

}

/* ------------------------------------------------------------ */
/***	OledClearBuffer
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display memory buffer.
*/

void
OledClearBuffer()
	{
	int			ib;
	BYTE *		pb;

	pb = rgbOledBmp;

	/* Fill the memory buffer with 0.
	*/
	for (ib = 0; ib < cbOledDispMax; ib++) {
		*pb++ = 0x00;
	}

}

/* ------------------------------------------------------------ */
/***	OledUpdate
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Update the OLED display with the contents of the memory buffer
*/

void
OledUpdate()
	{
	int		ipag;
//	int		icol;           // ALA unused
	BYTE *	pb;

	pb = rgbOledBmp;

	for (ipag = 0; ipag < cpagOledMax; ipag++) {

		PORTClearBits(prtDataCmd, bitDataCmd);

		/* Set the page address
		*/
		Spi1PutByte(0x22);		//Set page command
		Spi1PutByte(ipag);		//page number

		/* Start at the left column
		*/
		Spi1PutByte(0x00);		//set low nybble of column
		Spi1PutByte(0x10);		//set high nybble of column

		PORTSetBits(prtDataCmd, bitDataCmd);

		/* Copy this memory page of display data.
		*/
		OledPutBuffer(ccolOledMax, pb);
		pb += ccolOledMax;

	}

}

/* ------------------------------------------------------------ */
/***	OledPutBuffer
**
**	Parameters:
**		cb		- number of bytes to send/receive
**		rgbTx	- pointer to the buffer to send
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Send the bytes specified in rgbTx to the slave and return
**		the bytes read from the slave in rgbRx
*/

void
OledPutBuffer(int cb, BYTE * rgbTx)
	{
	int		ib;
	BYTE	bTmp;

	/* Bring the slave select line low
	*/
	PORTClearBits(prtSelect, bitSelect);

	/* Write/Read the data
	*/
	for (ib = 0; ib < cb; ib++) {
		/* Wait for transmitter to be ready
		*/
		while (SPI1STATbits.SPITBE == 0);

		/* Write the next transmit byte.
		*/
		SPI1BUF = *rgbTx++;

		/* Wait for receive byte.
		*/
		while (SPI1STATbits.SPIRBF == 0);
		bTmp = SPI1BUF;

	}

	/* Bring the slave select line high
	*/
	PORTSetBits(prtSelect, bitSelect);
	
}

/* ------------------------------------------------------------ */
/***	Spi1PutByte
**
**	Parameters:
**		bVal		- byte value to write
**
**	Return Value:
**		Returns byte read
**
**	Errors:
**		none
**
**	Description:
**		Write/Read a byte on SPI port 2
*/

BYTE
Spi1PutByte(BYTE bVal)
	{
	BYTE	bRx;

	/* Bring the slave select line low
	*/
	PORTClearBits(prtSelect, bitSelect);

	/* Wait for transmitter to be ready
	*/
	while (SPI1STATbits.SPITBE == 0);

	/* Write the next transmit byte.
	*/
	SPI1BUF = bVal;

	/* Wait for receive byte.
	*/
	while (SPI1STATbits.SPIRBF == 0);

	/* Put the received byte in the buffer.
	*/
	bRx = SPI1BUF;

	/* Bring the slave select line high
	*/
	PORTSetBits(prtSelect, bitSelect);
	
	return bRx;

}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

