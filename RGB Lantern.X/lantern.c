/***********************************************************************************************/
/*	PIC-Lantern																					*/
/*	Open Engineering, LLC dba AeroDyne Labs							
/*	www.aerodynelabs.com
 * Copyright (c) <2012> <Matthew E. NElson>

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
/*																								*/
/*	Discussion:																					*/
/*				*/
/*																								*/
/*	Future work:																				*/
/*	
/*																								*/
/*																								*/
/*  Pin diagram for 18F2520
 * Hardware Revision 1
Port A			Port B				Port C			
RA0 - PWR LED		RB0 - Mode select		RC0
RA1 - Light sensor	RB1 - Mode select		RC1
RA2 - Temp Sensor	RB2 - Mode select               RC2
RA3 - Humidity		RB3				RC3 - I2C Clk
RA4 - Pressure		RB4				RC4 - I2C Data
RA5 - 			RB5				RC5
RA6 - 			RB6 - ICSP			RC6 - Serial TX
RA7 - 			RB7 - ICSP			RC7 - Serial RX   */


/************************************************************************************************/
/*	Revision: 1.0																				*/
/*																							*/
/*	Revision History:																			*/
/*		1.0 - Initial design/program - MEN   													*/
/*		
/*		
/*		
/*		
/*																								*/
/*																								*/
/*																								*/
/************************************************************************************************/

#include <p18f2520.h>
#include <usart.h>
#include <stdio.h>
#include <adc.h>
#include <delays.h>
#include <math.h>
#include "blinkm.h"
#include "lantern.h"

//Definitions
//========================================================
#define MULTIPLIER 100  	//Used for printing out floating point numbers
#define CPU_LED LATAbits.LATA0  //Define the red LED on the Olimex as the CPU LED
#define FW_VERSION  56         //Firmware version

//Configuration Bits
//With MPLAB X, configuration bits must be set in code, all configuration options are below

#pragma config OSC = INTIO67    //Set to internal OSC
#pragma config WDT = OFF        //Turn off Watchdog timer
#pragma config LVP = OFF        //Turn off Low Volt. Programming
#pragma config BOREN = OFF      //Turn off brown out detection


//Global Variables Declarations
//========================================================
int Blink_B = 0x00;         //Broadcast address for BlinkM modules
int RGB_0 = 0x12;           //address of the BlinkM Module, node 0, it's actually 0x09, but it's left shifted
int RGB_1 = 0x13;           //address of node 1, humidity
int RGB_2 = 0x14;           //address of node 2
int RGB_3 = 0x15;           //address of node 3
int RGB_4 = 0x16;           //address of node 4
float temp_sensor = 0;      //Initialize temp sensor
int light_sensor = 0;       //Initialize light_sensor
int humidity_sensor = 0;    //Initialize humidity sensor
int pressure_sensor = 0;    //Initialize pressure sensor
int leftfloat = 0;          //Stores left of decimal place
int rightfloat = 0;         //Stores right of decimal
int rx_flag = 0;            //This flag indicates if we have received data
int color_flag = 0;         //Flag for color, if high then we ignore the RGB routine
char out_temp[5];           //Temp out array
char i = 0;
				
//Lookup tables, these have the RGB values, the temperature range is from 0 to 110 degrees F, room temp is around Green, 0 is Blue and 110 is Red
const rom unsigned int Red_LookupTable[110] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255};
const rom unsigned int Green_LookupTable[110] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255,250,245,240,235,230,225,220,215,210,205,200,195,190,185,180,175,170,165,160,155,150,145,140,135,130,125,120,115,110,105,100,95,90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,0,0,0,0,0,0,0,0};
const rom unsigned int Blue_LookupTable[110] = {255,250,245,240,235,230,225,220,215,210,205,200,195,190,185,180,175,170,165,160,155,150,145,140,135,130,125,120,115,110,105,100,95,90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//Function Declarations
//========================================================
//All functions except the ISR are declared in their respective header file
//Declare the serial RX function for the ISR
void rx_handler (void);  //Declare the ISR function


//Interrupts
//========================================================
/*When we press a key, we will interrupt and act on that key press */
#pragma code rx_interrupt = 0x8  
void rx_int (void) 
 {
  _asm goto rx_handler _endasm 
 }

/*  RX interrupt handler */
#pragma code
#pragma interrupt rx_handler 
 void rx_handler (void) 
 {
    while(!DataRdyUSART());
    i = ReadUSART();            	// read a single character from buffer
    PIR1bits.RCIF = 0;                  //reset the ISR flag.
    if ( 27 == i ){
    printf((const far rom char*)"\n\r>\n\rThat was the ESC key ASCII# %d - rebooting!\n\r",27);
    Reset();
 }
    else
    printf((const far rom char*)"\n\r>\n\rNothing to see here...\n\r",27);
    PIR1bits.RCIF = 0; 		//reset the ISR flag.
    process_command(i);             //calling a function in an ISR is not ideal, but it works
    //PIR1bits.RCIF = 0; 		//reset the ISR flag.
    return;
 }

//Main Program
//========================================================
void main (void)
{
//Startup and Init
    OSCCON = 0b01110000;        //8MHz; using the internal clock
    TRISA = 0b111111110;        //PORTA set RA0 as output, the rest inputs for A/D
    TRISB = 0;                  //PORTB set as outputs
    LATA = 0;                   //Sets all bits low on PORTA
    LATB = 0;                   //Sets all bits low on PORTB
    TRISC = 0xC0;               // turn on tri-state register and make all output pins
    PORTC = 0x00;               // make all output pins LOW

    //Init USART, set to 19,200, turn on RX Interrupt
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 51);
    RCONbits.IPEN = 1;          //Enable interrupt priority
    IPR1bits.RCIP = 1;          //Make receive interrupt high priority
    INTCONbits.GIEH = 1;        //Enable all high priority interrupts
    Delay10KTCYx(255);          //This gives us time for the ATTiny to boot up so it will listen to commands
    CPU_LED = 1;                //turns on power LED (RED LED on OLIMEX board)
    set_BlinkM_addr (RGB_1);
    set_color_now(RGB_1,0,0,255);
    main_menu();                //Display the main menu on the serial port
    reset:                      //Reset goto, if we press the ESC key, we reset to this point
    stop_script ();
    set_color_now(RGB_0,255,0,0);
    fade_speed (RGB_0,2);
    fade_to_color(RGB_0,0,0,255);
    Delay10KTCYx(255);
//Begin infinite loop
    while(1)  {
//Here we are starting off by putting the lantern in RGB Temp color mode.  This is just a bootstrap, the serial interrupt takes care of the rest
    if (color_flag == 0)         //If the color flag is set high we don't execute the Color temp RGB routine
        weather_mode();
    }
}
//Functions
//========================================================


//This displays the menu command through the serial port
void main_menu (void)
{
    printf((const far rom char*)"\n\n\rAERODYNE LABS UNIFIED OPERATING SYSTEM\n\n\r");
    printf((const far rom char*)"Copyright 2010-2012 AERODYNE LABS\n\r");
    printf((const far rom char*)"            -RGB Lantern\n\r");
    printf((const far rom char*)"Press M to put Lantern in Mood lighting mode\n\r");
    printf((const far rom char*)"Press C to put Lantern in Candle mode\n\r");
    printf((const far rom char*)"Press S to put Lantern in Seasons mode\n\r");
    printf((const far rom char*)"Press W to put Lantern in Thunderstorm mode\n\r");
    printf((const far rom char*)"Press E to stop execution of a mode\n\r");
    printf((const far rom char*)"Press T to read back the temperature\n\r");
    printf((const far rom char*)"Press F to read temp and set LED by temp value\n\r");
    printf((const far rom char*)"Press A to go into weather mode\n\r");
    printf((const far rom char*)"Press L to read back the light sensor value\n\r");
    printf((const far rom char*)"Press 0 (zero) to fade off the RGB LED\n\r");
    printf((const far rom char*)"Press H to bring these instructions up again.\n\n\r");
    printf((const far rom char*)"LANTERN>");
}

//Once we have a command, process it
void process_command (char i)
{
    printf((const far rom char*)"\n\rYou pressed %c\n\r",i,i);
    if ( ('0' <= i) && (i <= '9') ){
    printf((const far rom char*)"Confirmed digit pressed...\n\r");
    switch (i) {
        case '0':
            printf((const far rom char*)"RGB LED Fading to off \n\r");
            color_flag = 1;
            fade_to_color(RGB_0,0,0,0);
            printf((const far rom char*)"RGB LED is now off.\n\r");
            break;
        default:
            printf((const far rom char*)"FUFUFUFUFUFUFUFUFU \n\r");
	}		
    }
    else
	if ( ('H' == i) || (i == 'h') ){
	main_menu();
	}
    else
    if ( ('S' == i) || (i == 's') ){
        color_flag = 1;
        printf((const far rom char*)"Setting the lantern to Season's mode...\n\r");
        start_script (RGB_0,15);
	printf((const far rom char*)"Now in Season mode...pretty\n\r");
        putrsUSART((const far rom char*)"\n\r");
    }
    else
	if ( ('M' == i) || (i == 'm') ){
        color_flag = 1;
        printf((const far rom char*)"Setting the lantern to Mood lighting mode...\n\r");
        start_script (RGB_0,11);
	printf((const far rom char*)"Now in Mood lighting mode...RAWR\n\r");
        putrsUSART((const far rom char*)"\n\r");
	}
    else
        if ( ('C' == i) || (i == 'c') ){
	color_flag = 1;
        printf((const far rom char*)"Setting the lantern to Candle mode...\n\r");
        start_script (RGB_0,12);
        start_script (RGB_1,12);
	printf((const far rom char*)"Now in Candle..OH GOD I'M ON FIRE...oh wait, its not real..mode\n\r");
        putrsUSART((const far rom char*)"\n\r");
	}
    else
	if ( ('E' == i) || (i == 'e') ){
	color_flag = 1;
        printf((const far rom char*)"Stopping any running script...\n\r");
        stop_script ();
	printf((const far rom char*)"Script stopped.\n\r");
        putrsUSART((const far rom char*)"\n\r");
	}
    else
	if ( ('W' == i) || (i == 'w') ){
	color_flag = 1;
	printf((const far rom char*)"Setting the lantern to Thunderstorm mode...\n\r");
	start_script (RGB_0,16);
	putrsUSART((const far rom char*)"\n\r");
	}
    else
        if ( ('T' == i ) || (i == 't') ){
	printf((const far rom char*)"Reading the temp\n\r");
	temp_sensor = read_temp();
	leftfloat = (int)temp_sensor;  //Stores the values from the left side of the decimal
	rightfloat = (int)((temp_sensor - leftfloat) * MULTIPLIER);
	fprintf(_H_USART, (const far rom char*)"Temp:%d.%03d|",leftfloat,rightfloat, " \n\r");
	}
  else
        if ( ('A' == i ) || (i == 'a') ){
	printf((const far rom char*)"Weather Mode!  \n\n\r");
        weather_mode();
	}
    else
        if ( ('F' == i ) || (i == 'f') ){
        printf((const far rom char*)"Setting to TEMP->RGB Mode \n\r");
        color_flag = 0;
	set_temp_color();
	}
    else
	if ( ('L' == i ) || (i == 'l') ){
        printf((const far rom char*)"Reading the light sensor\n\r");
        light_sensor = read_light();
	fprintf(_H_USART, (const far rom char*)"Light:%#u|",light_sensor, " \n\r");
	}
    else
        printf((const far rom char*)"I'm sorry Dave, but I can't do that.\n\r");
         //printf((const far rom char*)"PICLANTERN>");
}
//This funtion reads the temp, then looks up the the RGB values from the Red, Green and Blue tables
//It then sets the color and sends the temp data on the serial port
void set_temp_color (void)
{
    stop_script();
    CPU_LED = ~CPU_LED;  			//Toggle the CPU_LED
    temp_sensor = read_temp();
    leftfloat = (int)temp_sensor;  //Stores the values from the left side of the decimal
    fade_to_color(RGB_0,Red_LookupTable[leftfloat],Green_LookupTable[leftfloat],Blue_LookupTable[leftfloat]);
    fprintf(_H_USART, (const far rom char*)"Temp:%#u|",leftfloat, " \n\r");
    Delay10KTCYx(128);
}	

void weather_mode (void)
{
    stop_script();
    CPU_LED = ~CPU_LED;  			//Toggle the CPU_LED
    temp_sensor = read_temp();
    humidity_sensor = read_humidity();
    pressure_sensor = read_pressure();
    light_sensor = read_light();
    leftfloat = (int)temp_sensor;  //Stores the values from the left side of the decimal
    fade_to_color(RGB_0,Red_LookupTable[leftfloat],Green_LookupTable[leftfloat],Blue_LookupTable[leftfloat]);
    fprintf(_H_USART, (const far rom char*)"$RGB,%#u,%#u,%#u,%#u,%#u,#\n\r",FW_VERSION,leftfloat,humidity_sensor,pressure_sensor,light_sensor);
    Delay10KTCYx(128);
}
/*  Read the temp sensor from the A/D and return the temp in degrees F */
float read_temp (void)
{
    int adc_value = 0;
    float kelvin = 0;
    float celsius = 0;
    float fahrenheit = 0;
    //Configures the A/D, selects Channel 2 (RA2) and sets VREF to VDD and VSS (5V and 0V)
    OpenADC (ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_12_TAD, ADC_CH2 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0);
    ConvertADC();		//Call the ADC function
    while(BusyADC());		//Wait for the A/D process to finish
    adc_value = ReadADC();  	//Read the ADC
    kelvin = ((((float)adc_value/1023) * 5) * 100);
    celsius = kelvin - 273;
    //Note:  For now subtracting 4, slight offset with the temp measurement
    fahrenheit = ((celsius*(1.8))+32)-4;

    return fahrenheit;
}	
/*  Read the light sensor from the A/D and return the 10-bit value */

int read_light (void)
{
    int adc_value = 0;
    //Configures the A/D, selects Channel 1 (RA1) and sets VREF to VDD and VSS (5V and 0V)
    OpenADC (ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_12_TAD, ADC_CH1 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0);
    ConvertADC();		//Call the ADC function
    while(BusyADC());		//Wait for the A/D process to finish
    adc_value = ReadADC();  	//Read the ADC
    return adc_value;
}

int read_humidity (void)
{
    int adc_value = 0;
    //Configures the A/D, selects Channel 1 (RA3) and sets VREF to VDD and VSS (5V and 0V)
    OpenADC (ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_12_TAD, ADC_CH2 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0);
    ConvertADC();				//Call the ADC function
    while(BusyADC());			//Wait for the A/D process to finish
    adc_value = ReadADC();  	//Read the ADC
    return adc_value;
}

int read_pressure (void)
{
    int adc_value = 0;
    //Configures the A/D, selects Channel 1 (RA4) and sets VREF to VDD and VSS (5V and 0V)
    OpenADC (ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_12_TAD, ADC_CH3 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 0);
    ConvertADC();				//Call the ADC function
    while(BusyADC());			//Wait for the A/D process to finish
    adc_value = ReadADC();  	//Read the ADC
    return adc_value;
}