/************************************************************************************************/
/*	PIC-Lantern																					*/
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
/*	Discussion:																					*/
/*				*/
/*																								*/
/*	Future work:																			*/
/*
/*																								*/
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
