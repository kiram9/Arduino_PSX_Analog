Analog PSX Controller Decoder Library (Psx_analog.h)
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
