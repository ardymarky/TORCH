typedef union WheelReading{
  float angle_uart;
  byte byteArray[4]; /* you can use other variable types if you want. Like: a 32bit integer if you have 4 8bit variables in your struct */
};

uint8_t WE_state = 0;
WheelReading wheel_reading;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(115200);
}

void loop() {
  if (Serial2.available()) {
    readWE_data();
  }
  if (Serial3.available()) {
    readRF_data();
  }
}

void readWE_data() {

    uint8_t WE_state = 0;
    uint8_t byte_num = 0;

    while (1) {

      if (Serial2.available()) {

        uint8_t val_1 = Serial2.read();

        if (WE_state == 2 && byte_num < 4) {
          wheel_reading.byteArray[byte_num] = val_1;
          byte_num += 1;
        }
        else if (WE_state == 2 && byte_num >= 4) {
          WE_state = 0;
          byte_num = 0;
          break;
        }
        else if (val_1 == 0x69 && WE_state == 0) {
          WE_state = 1;
          
        }
        else if (val_1 == 0x69 && WE_state == 1) {
          WE_state = 2;
        }
        else {
          WE_state = 0;
        }      
      }
    }

    unsigned long curMillis = millis();
    // Serial.print("Wheel reading: "); 
    Serial.print(wheel_reading.angle_uart);
    Serial.print("\t");
    Serial.println(curMillis);
}


void readRF_data() {
  int frame_size = 7;

    uint8_t frame[frame_size];
    uint8_t lastChar = 0x00; 

    while (1) {

      if (Serial3.available()) {      
        uint8_t curChar = Serial3.read();

        if ((lastChar == 0x59) && (curChar == 0x59)) {
          // Break to begin frame
          break;
          
        } else {
          // We have not seen two 0x59's in a row -- store the current character and continue reading.         
          lastChar = curChar;
        }           
      }
    }

    uint8_t checksum = 0x59 + 0x59;
    for (int i=0; i<frame_size; i++) {
      // Read one character
      while (!Serial3.available()) {
        // wait for a character to become available
      }    
      frame[i] = Serial3.read();

      // Store running checksum
      if (i < frame_size-2) {
        checksum += frame[i];
      }
    }

    // Step 3: Interpret frame
    uint16_t dist = (frame[1] << 8) + frame[0];
    uint16_t st = (frame[3] << 8) + frame[2];
    uint8_t reserved = frame[4];
    uint8_t originalSignalQuality = frame[5];


    // Step 4: Store values
    int distance = dist;
    int strength = st;

    unsigned long curMillis = millis();
    Serial.print("Rangfinder:");
    Serial.print("\t");
    Serial.print(distance);
    Serial.print(" cm\t");
    // Serial.print(strength);
    // Serial.print("\t");
    Serial.println(curMillis);
}