/*  Analog PSX Controller Decoder Library (Psx_analog.cpp)
	Written by: Kieran Levin Mar 8th, 2009
	Based upon the work by Kevin Ahrendt June 22nd, 2008

	Analog protocal implemented with information from 
	http://www.curiousinventor.com/guides/ps2

		Fixed reverse shifting in (MSB->LSB) now reads analog values....
	Controller protocol implemented using Andrew J McCubbin's analysis.
	http://www.gamesx.com/controldata/psxcont/psxcont.htm
	
	Shift command is based on tutorial examples for ShiftIn and ShiftOut
	functions both written by Carlyn Maw and Tom Igoe
	http://www.arduino.cc/en/Tutorial/ShiftIn
	http://www.arduino.cc/en/Tutorial/ShiftOut
	
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
    Standard Digital Pad

    BYTE    CMND    DATA

     01     0x01    idle
     02     0x42    0x41
     03     idle    0x5A    Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
     04     idle    data    SLCT           STRT UP   RGHT DOWN LEFT
     05     idle    data    L2   R2    L1  R1   /\   O    X    |_|

    All Buttons active low.

--------------------------------------------------------------------------------
    Analogue Controller in Red Mode

    BYTE    CMND    DATA

     01     0x01    idle
     02     0x42    0x73
     03     idle    0x5A    Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
     04     idle    data    SLCT JOYR JOYL STRT UP   RGHT DOWN LEFT
     05     idle    data    L2   R2   L1   R1   /\   O    X    |_|
     06     idle    data    Right Joy 0x00 = Left  0xFF = Right
     07     idle    data    Right Joy 0x00 = Up    0xFF = Down
     08     idle    data    Left Joy  0x00 = Left  0xFF = Right
     09     idle    data    Left Joy  0x00 = Up    0xFF = Down
--------------------------------------------------------------------------------
*/
#include "Psx_analog.h"
#include "Spi.h"
Psx::Psx()
{
}
#define CLKDELAY 20

#define WAIT 100
byte Psx::shift(byte _dataOut) { return Spi.transfer(_dataOut,WAIT);}
/*
byte Psx::shift(byte _dataOut)	// Does the actual shifting, both in and out simultaneously
{


	_temp = 0;
	_dataIn = 0;

	for (_i = 0; _i <= 7; _i++)
	{
		digitalWrite(_clockPin, LOW);
		if ( _dataOut & (1 << _i) ) digitalWrite(_cmndPin, HIGH);	// Writes out the _dataOut bits
		else digitalWrite(_cmndPin, LOW);
		_temp = digitalRead(_dataPin);					// Reads the data pin
		if (_temp)
		{
			_dataIn = _dataIn | (B00000001 << _i);		// Shifts the read data into _dataIn
		}

		digitalWrite(_clockPin, HIGH);
	}
	return _dataIn;
}
*/

void Psx::setupPins(byte dataPin, byte cmndPin, byte attPin, byte clockPin)
{
	pinMode(dataPin, INPUT);
	digitalWrite(dataPin, HIGH);	// Turn on internal pull-up
	_dataPin = dataPin;

	pinMode(cmndPin, OUTPUT);
	_cmndPin = cmndPin;

	pinMode(clockPin, OUTPUT);
	_clockPin = clockPin;
	digitalWrite(_clockPin, HIGH);

	pinMode(attPin, OUTPUT);
	_attPin = attPin;
	digitalWrite(_attPin, HIGH);

	
	Motorsmall = 0x00;
	Motorlarge = 0x00;
	//setup SPI 
  	Spi.mode((1<<SPR1) | (1<<SPR0) | (1<<DORD) | (1<<CPOL) | (1<<CPHA)); 
  	SPSR = SPSR | (0<<SPI2X);

}

unsigned int Psx::poll()
{
	digitalWrite(_attPin, LOW);

	shift(0x01);
	Controller_mode = shift(0x42);
	shift(0x00);

	digital_buttons = (~shift(Motorsmall) & 0x00FF); //motor (change value to FF to turn on motor in dualshock controller) 
	digital_buttons |= (~shift(Motorlarge) << 8); //motor (Large motor, will turn on for values over 40)
	if (0x70 == (Controller_mode & 0xF0))
	{
		Right_x = shift(0x00);
		Right_y = shift(0x00);
		Left_x = shift(0x00);
		Left_y = shift(0x00);
	}


	digitalWrite(_attPin, HIGH);

	return digital_buttons;
}

void Psx::Config_Enter()
{
	digitalWrite(_attPin, LOW); //goto configuration mode 
	shift(0x01);
	shift(0x43);
	shift(0x00);
	shift(0x01);
	shift(0x00);	
	digitalWrite(_attPin, HIGH);
}

void Psx::Config_Exit()
{
	digitalWrite(_attPin, LOW); //exit config mode 
	shift(0x01);
	shift(0x43);
	shift(0x00);
	shift(0x00);
	shift(0x5A);	
	shift(0x5A);
	shift(0x5A);
	shift(0x5A);
	shift(0x5A);	
	digitalWrite(_attPin, HIGH);
}
//modes are 0x01 analog| 0x00 digital
void Psx::Config_Mode(byte mode)
{
	digitalWrite(_attPin, LOW); 
	shift(0x01);
	shift(0x44);
	shift(0x00);
	shift(mode);
	shift(0x03);	//lock analog/digital mode select button on controller
	shift(0x00);
	shift(0x00);
	shift(0x00);
	shift(0x00);	
	digitalWrite(_attPin, HIGH);	
}

void Psx::Config_dualshock()
{
	digitalWrite(_attPin, LOW); //setup motor command mapping 
	shift(0x01);
	shift(0x4D);
	shift(0x00);
	shift(0x00);
	shift(0x01);	
	shift(0xFF);
	shift(0xFF);
	shift(0xFF);
	shift(0xFF);	
	digitalWrite(_attPin, HIGH);
}

byte Psx::initcontroller(byte controller_mode)
{
	poll();
	Config_Enter();
	Config_Enter();
	Config_Mode(controller_mode); 
	Config_Mode(controller_mode); 
	Config_dualshock();
	Config_Exit();
	Config_Exit();
	Config_Exit();

	Config_Exit();
	poll();
	poll();
	poll();
	poll();
	return Controller_mode; 	
}


