/*  Analog PSX Controller Decoder Library (Psx_analog.h)
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


#ifndef Psx_h
#define Psx_h

//#include "WProgram.h"
#include "Arduino.h"
// Button Hex Representations:
//hat 
#define psxLeft		0x0080  
#define psxDown		0x0040 
#define psxRight	0x0020 
#define psxUp		0x0010 
#define psxStrt		0x0008 
#define psxSlct		0x0001 

//buttons
#define psxSqu		0x8000 
#define psxX		0x4000 
#define psxO		0x2000 
#define psxTri		0x1000 
#define psxR1		0x0800 
#define psxL1		0x0400 
#define psxL2		0x0100 
#define psxR2		0x0200 
#define psxJoyL		0x0002
#define psxJoyR		0x0004
//other defines
#define psxAnalog	0x01
#define psxDigital	0x00



class Psx
{
	public:
		
		Psx();
		void setupPins(byte, byte, byte, byte);		// (Data Pin #, CMND Pin #, ATT Pin #, CLK Pin #, Delay)
									// Delay is how long the clock goes without changing state
									// in Microseconds. It can be lowered to increase response,
									// but if it is too low it may cause glitches and have some
									// keys spill over with false-positives. A regular PSX controller
									// works fine at 50 uSeconds.
                												
		unsigned int poll();				// Returns the status of the button presses in an unsignd int.
									// The value returned corresponds to each key as defined above.
		
		byte initcontroller(byte); 		//initialize the controller (psxAnalog or psxDigital)
									//if controller is set to digital the right and left values in the Psx_Response
									//may be invalid
		byte Motorsmall;
		byte Motorlarge;			//set the vibration motor feedback level for dualshock controllers 
									//(Small motor, Large motor) Note the small motor will only turn on with
									//a value of 0xFF


		//note these are dangerous since these are unprotected, and they should be but this method this allows a faster program 
		//because we dont need the overhead of function calls, (just be careful
		byte Controller_mode;
		unsigned int digital_buttons;
		byte Right_x;
		byte Right_y;
		byte Left_x;
		byte Left_y;
		
	private:
		void Config_dualshock();
		void Config_Mode(byte);
		void Config_Exit();
		void Config_Enter();

		byte shift(byte _dataOut);
		byte _dataPin;
		byte _cmndPin;
		byte _attPin;
		byte _clockPin;
		
		byte _delay;
		byte _i;
		boolean _temp;
		byte _dataIn;
		
		byte _data1;
		byte _data2;
};

#endif
