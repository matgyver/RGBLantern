/************************************************************************************************/
/*	BlinkM Library															
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
/*													
/*	Discussion:
 * This library contains functions for the BlinkM module					
/*				*/
/*																
/*	Future work:										
/*
/*														
/*
 *
 * Header File
 *
 */
//Functions
//Set the BlinkM Addr, pass in the BlinkM addr in hex
void set_BlinkM_addr (int BlinkM_addr);
//Read the address of the BlinkM Module
int read_BlinkM_addr (void);
//Set the color now, pass in address and RGB values are int from 0 to 255
void set_color_now (int addr, int R, int G, int B);
//Stop any script running on the BlinkM
void stop_script (void);
//Set the Fade speed, pass in address and int value passed in 0 to 255
void fade_speed (int addr, int fade_sp);
//Fade to a RGB color, RGB values are int from 0 to 255
void fade_to_color (int addr, int R, int G, int B);
//Start a pre-programmed script on the BlinkM, see documentation
void start_script (int addr, int script_num);
