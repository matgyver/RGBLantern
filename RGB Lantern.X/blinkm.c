/************************************************************************************************/
/*	BlinkM Library																					*/
/*	Open Engineering, LLC dba AeroDyne Labs							 							*/
/*	www.aerodynelabs.com																		*/
/*																								*/
/*	All code is copyright 2011 by the author, Matthew E. Nelson									*/
/*	Unless authorized by the author, permission to redistribute the code is given solely 		*/
/*	to Open Engineering, LLC. dba AeroDyne Labs													*/
/*  For questions on redistribution, please contact Matthew Nelson at mnelson@aerodynelabs.com  */
/*																								*/
/*																								*/
/*																								*/
/*	Discussion:
 * This library contains functions for the BlinkM module																				*/
/*				*/
/*																								*/
/*	Future work:																			*/
/*
/*																								*/
/*
 *
 * C File
 *
 *
/* This function sets the BlinkM address.  This will ensure that the BlinkM
/* is set to the address you want to use.  NOTE:  If you have multiple BlinkM
/* on the board it will set ALL BlinkM to the same address.  Power down the other
/* BlinkMs before running this code.  */

/*Revision Update
 1.1 - Added support for all functions except the stop script so that you now
 need to pass in the address of the blinkM module you want to talk to
 1.2 - Added support to read an address from a module for diagnostic reasons
 */

#include <i2c.h>

void set_BlinkM_addr (int BlinkM_addr)
{
    OpenI2C(MASTER, SLEW_ON);		//Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();				//Start the I2C bus with the start bit
    IdleI2C();				//I2C Idle time
    WriteI2C(0x00);                     //This sends the broadcast address of 0x00
    IdleI2C();				//Idle command, it ensures the I2C is not busy
    WriteI2C('A');			//To set the BlinkM address, we send the command in the following format
    IdleI2C();                      	// 'A',a,0xd0,0x0d,a where 'A' is the char telling the BlinkM to set address
    WriteI2C(BlinkM_addr);    		// a is the address you wish to set and 0xd0 and 0x0d is a check to ensure
    IdleI2C();                          // an address doesnt' get set by accident.  See pg 21 of the BlinkM documentation
    WriteI2C(0xd0);
    IdleI2C();
    WriteI2C(0x0d);
    IdleI2C();
    WriteI2C(BlinkM_addr);		//We send the addrress one more time, this will actually set the address on the BlinkM
    IdleI2C();
    StopI2C();
}

int read_BlinkM_addr (void)
{
    int addr = 0xff;
    OpenI2C(MASTER, SLEW_ON);		//Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();				//Start the I2C bus with the start bit
    IdleI2C();				//I2C Idle time
    WriteI2C(0x00);                     //This sends the broadcast address of 0x00
    IdleI2C();				//Idle command, it ensures the I2C is not busy
    WriteI2C('a');			//get the address
    IdleI2C();
    addr = ReadI2C();
    IdleI2C();
    StopI2C();
    return addr;
}

/*  This function will set a specific color right away.  Pass in the R G B values as int from
/*  0 to 255.  */

void set_color_now	(int addr, int R, int G, int B)
{
    OpenI2C(MASTER, SLEW_ON);		//Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();
    IdleI2C();
    WriteI2C(addr);			//The Address of the BlinkM is 0x12, well actually it's
    IdleI2C();                      	//0x09, but the PIC uses a left-shifted display so it's
    WriteI2C('n'); 			//multiplied by 2 so 0x12.  Yeah, this screwed me up too
    WriteI2C(R);
    WriteI2C(G);
    WriteI2C(B);
    StopI2C();
}

/* This function issues the stop scipt command.  The BlinkM is able to execute pre-programmed
/* scripts and by default the BlinkM will start playing script 0 on startup
   This code is set to stop ALL BlinkM modules												*/

void stop_script (void)
{
    OpenI2C(MASTER, SLEW_ON);		//Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();				//Start the I2C bus with the start bit
    IdleI2C();
    WriteI2C(0x00);			//Broadcast address so that all BlinkM modules stop
    IdleI2C();				//Idle command, it ensures the I2C is not busy
    WriteI2C('o');			//To stop the script we send an 'o' character
    IdleI2C();
    StopI2C();
}

/* This function will start playing one of the pre-programmed scripts.  The BlinkM is able to execute pre-programmed
/* scripts and by default the BlinkM will start playing script 0 on startup */

//The following is the default pre-programmmed scripts and the number to pass into this function to call that script
/* id description 					color sequence
------------------------------------------------
0 eeprom scriptdefault startup 	white ? red ? green ? blue ? off
1 RGB 						red ? green ? blue
2 white ?ash 					white ? off
3 red ?ash 					red ? off
4 green ?ash 					green ? off
5 blue ?ash 					blue ? off
6 cyan ?ash 					cyan ? off
7 magenta ?ash 				magenta ? off
8 yellow ?ash 					yellow ? off
9 black 						off
10 hue cycle 					red ? yellow ? green ? cyan ? blue ? purple
11 mood light 					random hue ? random hue
12 virtual candle 				random yellows
13 water re?ections 			random blues
14 old neon 					random orangeish reds
15 the seasons 				spring colors ? summer ? fall ? winter
16 thunderstorm 				random blues & purples ? white ?ashes
17 stop light 					red ? green ? yellow
18 morse code 				S.O.S in white  */

void start_script (int addr, int script_num)
{
    OpenI2C(MASTER, SLEW_ON);		//Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();				//Start the I2C bus with the start bit
    IdleI2C();
    WriteI2C(addr);			//The address of the BlinkM is 0x12
    IdleI2C();				//Idle command, it ensures the I2C is not busy
    WriteI2C('p');			//To start playing a script we send a 'p'
    IdleI2C();
    WriteI2C(script_num);               //This passes in the script number to play
    IdleI2C();
    WriteI2C(0);			//Setting to zero has the script play forever
    IdleI2C();
    WriteI2C(0);			//Setting this to zero means to start the script from the beggining
    StopI2C();
}

/* This function takes in an integer value from 1-255 (value 0 is reserved).  This then sets the speed
/* when you call the fade to color function.  255 is the fatest (instant) and 1 is the slowest */

void fade_speed (int addr, int fade_sp)
{
    OpenI2C(MASTER, SLEW_ON);           //Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();
    IdleI2C();
    WriteI2C(addr);			//The Address of the BlinkM is 0x12, well actually it's
    IdleI2C();                          //0x09, but the PIC uses a left-shifted display so it's
    WriteI2C('f'); 			//multiplied by 2 so 0x12.  Yeah, this screwed me up too
    WriteI2C(fade_sp);
    StopI2C();
}

/*  This function will set a specific color but will fade to that color.  Pass in the R G B values as int from
/*  0 to 255.  */

void fade_to_color (int addr, int R, int G, int B)
{
    OpenI2C(MASTER, SLEW_ON);           //Open I2C in MASTER mode, Slew off
    SSPADD = 19;			//I2C clock, ((FOSC/100kHz) / 4) - 1
    StartI2C();
    IdleI2C();
    WriteI2C(addr);			//The Address of the BlinkM is 0x12, well actually it's
    IdleI2C();                          //0x09, but the PIC uses a left-shifted display so it's
    WriteI2C('c'); 			//multiplied by 2 so 0x12.  Yeah, this screwed me up too
    WriteI2C(R);
    WriteI2C(G);
    WriteI2C(B);
    StopI2C();
}