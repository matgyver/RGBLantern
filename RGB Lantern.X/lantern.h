/************************************************************************************************/
/*	PIC-Lantern																
/*	Open Engineering, LLC dba AeroDyne Labs							
/*	www.aerodynelabs.com								
/*									
/*	Copyright (c) <2012> <Matthew E. NElson>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
/*																	
/*	Discussion:												
/*				*/
/*													
/*	Future work:												
/*
/*																
/*
 *
 * Lantern.h Header file
*/
//Function defines
//ASCII Serial port menu for control and debugging
void main_menu (void);
//Process an incoming char
void process_command (char);
//Function to read temp and set the color based on temp
void set_temp_color (void);
//Read the temp sensor and return a float value
float read_temp (void);
//Read the light sensor and return an int value from 0 to 1023
int read_light (void);

int read_humidity (void);

int read_pressure (void);
//Function for weather mode, in weather mode the up to 3 RGBs will be set
//The first will represent temp, second humidity and the third pressure
void weather_mode (void);
