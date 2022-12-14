/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"
#include <stdio.h>

#define chan *(volatile uint8_t *) 0x41220000
#define ctrl *(volatile uint8_t *) 0x41220008


//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};


// Here the setup operations for the LED matrix will be performed
void setup(){


	//reseting screen at start is a MUST to operation (Set RST-pin to 1).
	ctrl = 0;
	chan = 0;

	usleep(500);
	ctrl |= 0b00000001;

	for(uint8_t a = 0; a<24; a++) {
			uint8_t data = 0b11111111;	//uint8_t data = 0b00010000;
			for (uint8_t b = 0; b<6; b++) {
				if(data & 0b10000000) ctrl |= 0b00010000;	//SDA = 1
				else ctrl &= ~0b00010000;	//SDA = 0

				ctrl &= ~0b00001000;	//SCK = 0
				data <<= 1;
				ctrl |= 0b00001000;	//SCK = 1
			}
		}
	ctrl |= 0b00000100;

	latch();

}

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;

	//Write rest of two lines of code required to make this function work properly (green and red colors to array).
	dots[x][y][1]=g;
	dots[x][y][2]=r;


}

/* run(): Function should be called every time that TickHandler() is called (800 times/s). Run() is used to
transmit new values to screen in given X-column (variable x is used to indicate x-column). Pixel values
are taken from dots array.
*/

//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x){

	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array


	// First, we loop through the Y-rows ( Y0->Y7)
	// Then, we loop through B->G->R
	// Third, we loop through the single colors' data
	chan = 0b00000000;

	// First loop goes through Y-rows ( Y0-> Y7)
	for(uint8_t y=0; y<8; y++) {

		// Second loop goes through colors (order is B,G,R)
		for(uint8_t color=0; color<3; color++) {

			//Read dots array's value in this position to some temporary variable. this temporary variable is used in sending data
			uint8_t data = dots[x][y][color];

			// Third loop goes through actual color data
			for(uint8_t byte_count=0; byte_count<8; byte_count++) {

				if(data & 0x80){
					ctrl |= 0b00010000; //SDA = 1
					}
				else {
					ctrl &= ~0b00010000; //SDA = 0
					}

				ctrl &= ~0b00001000; //SCK = 1
				data <<= 1;
				ctrl |= 0b00001000;  //SCK = 0
			}
		}
	}
	ctrl &= ~0b00001000; //SCK = 0
	latch();
	open_line(x);

 }

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
	// Toggle lat bit
	ctrl |= 0b00000010;
	ctrl &= ~0b00000010;
}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){

	switch (x)
	{
		case 0:	chan = 0b00000001; break;
		case 1: chan = 0b00000010; break;
		case 2: chan = 0b00000100; break;
		case 3: chan = 0b00001000; break;
		case 4: chan = 0b00010000; break;
		case 5: chan = 0b00100000; break;
		case 6: chan = 0b01000000; break;
		case 7: chan = 0b10000000; break;

		default: chan = 0b00000000;
	}

}
