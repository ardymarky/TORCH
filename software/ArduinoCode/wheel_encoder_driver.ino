#include "SPI.h"

#include "MLX90316.h"

/* MLX90316 Rotary Position Sensor */
byte pinSS = 3;
byte pinSCK = 4;
byte pinMOSI = 5;
// float rough_gain= 0x8;
float angle;

//Metro mlxMetro = Metro(5); 
MLX90316 mlx_1 = MLX90316(); 

void setup(){
  Serial.begin(9600); 
  mlx_1.attach(pinSS,pinSCK,pinMOSI); 
  SPI.begin();
  // Serial.println("MLX90316 Rotary Position Sensor");
}

void loop() {
  
    angle = mlx_1.readAngle();
    // float rough_gain = SPI.transfer(0x8);
    // Serial.print("angle = ");Serial.println((float)angle/10);

    typedef union WheelReading{
      float angle_uart;
      byte byteArray[4]; /* you can use other variable types if you want. Like: a 32bit integer if you have 4 8bit variables in your struct */
    };

    WheelReading wheel_reading;

    wheel_reading.angle_uart = (float)angle/10;

    Serial.write(0x69);
    Serial.write(0x69);
    Serial.write(wheel_reading.byteArray[0]);
    Serial.write(wheel_reading.byteArray[1]);
    Serial.write(wheel_reading.byteArray[2]);
    Serial.write(wheel_reading.byteArray[3]);

    }
  
