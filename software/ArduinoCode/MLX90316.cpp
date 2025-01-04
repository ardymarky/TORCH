/*
  MLX90316.cpp -  Library to use with Melexis MLX90316 rotary encoder chip.
  Created by Martin Nawrath KHM 2010.
	Remix by Patrick Bouffel to ajust to Datasheet - 2018-01-27
  Released into the public domain.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "MLX90316.h"


// constructor
MLX90316::MLX90316() {
};

// attach
void MLX90316::attach(byte pinSS, byte pinSCK, byte pinMOSI ) {

	_pinSS = pinSS;
	_pinSCK = pinSCK;
	_pinMOSI = pinMOSI;

	pinMode(_pinSS , OUTPUT);
	pinMode(_pinSCK , OUTPUT);
	pinMode(_pinMOSI , OUTPUT);
}

//****************************************************** 
float MLX90316::readAngle() {

  byte bb;
  long int ret=-1;
  unsigned int rr=-1;
  unsigned int cc=-1;
  float test;

  digitalWrite(_pinSS,0);	// Select Chip

  _spiWByte(0x55);  // send sync byte ( AA reversed order = 55?)
  _spiWByte(0xFF);  // send sync byte FF)

	rr = (_spiRByte() << 8) + _spiRByte(); // receive 1. byte of mlx data
	cc = (_spiRByte() << 8) + _spiRByte(); // receive 1. byte of mlx data (inverted)

	if (rr+cc+1!=0) ret=-1;	// check if bit Errors
	else {

		if ((rr & 3) == 2) {
		  if ( rr & (1 << 4)) ret=-2;  // Magnetic Field Too Strong
		  if ( rr & (1 << 5)) ret=-3;  // Magnetic Field Too Weak
		}
    else if ((rr & 3) == 1) { // valid angle data ?
		  rr = (rr >> 2);
		  ret = rr ;
		  ret = ret *  3600 / 16384;	// scale output to 360 deg, untit in tens of deg.	
      test = float(rr) * float(3600) / float(16384);
	  }
	}
  digitalWrite(_pinSS,1);	// Deselect Chip

  return((float)test);

}

//*************************************************************
// send and receive SPI byte  to/from MLX90316 Chip
void MLX90316::_spiWByte(uint8_t tx) {

  pinMode(_pinMOSI,OUTPUT);   // switch MOSI pin to output

  for (int ix = 0; ix < 8; ix++){  // receive/transmit 8 SPI bits

    // write SPI transmit bit to sensor
    if ((tx & 1) != 0) digitalWrite(_pinMOSI,1); 
    							else digitalWrite(_pinMOSI,0);
    tx= ( tx >> 1);

    digitalWrite(_pinSCK,1);   // clocksignal positive slope
    digitalWrite(_pinSCK,0);   // clocksignal negative slope
  }
	delayMicroseconds(30);

	pinMode(_pinMOSI,INPUT_PULLUP);    // switch MOSI pin to input
}

//*************************************************************
// receive SPI byte  to/from MLX90316 Chip
uint8_t  MLX90316::_spiRByte() {

  byte rxb=0;

  for (int ix = 0; ix < 8; ix++){  // receive/transmit 8 SPI bits

    digitalWrite(_pinSCK,1);    // clocksignal positive slope
		delayMicroseconds(5);
    rxb = ( rxb << 1 );           // shift received byte left
    if (digitalRead(_pinMOSI) ==1) rxb = rxb | 1; // read respose bit from sensor
    digitalWrite(_pinSCK,0);   // clocksignal negative slope
  }
	delayMicroseconds(30);
  return(rxb);
}



