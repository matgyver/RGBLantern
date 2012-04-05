/************************************************************************************************/
/*	BlinkM Library																					*/
/*	Open Engineering, LLC dba AeroDyne Labs							 							*/
/*	www.aerodynelabs.com																		*/
/*																								*/
/*	All code is copyright 2011 by the author, Matthew E. Nelson                             */
/*	Unless authorized by the author, permission to redistribute the code is given solely 	*/
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
