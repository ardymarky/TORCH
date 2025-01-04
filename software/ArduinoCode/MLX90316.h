/*
  MLX90316.cpp -  Library to use with Melexis MLX90316 rotary encoder chip.
  Created by Martin Nawrath KHM 2010.
	Remix by Patrick Bouffel to ajust to Datasheet - 2018-01-27
  Released into the public domain.
*/

#ifndef MLX90316_h
#define MLX90316_h

class MLX90316
{
  public:
	
		MLX90316();			// constructor	
		void attach(byte pin1, byte pin2, byte pin3);	// attach

		float readAngle(); // read sensor angle value
	
  private:
   
		byte _pinSCK;
		byte _pinSS;
		byte _pinMOSI;

		void  _spiWByte(uint8_t  tx);
		uint8_t  _spiRByte();
};


#endif
