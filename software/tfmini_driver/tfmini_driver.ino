void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

}

void loop() {

  int frame_size = 7;

  uint8_t frame[frame_size];
  uint8_t lastChar = 0x00; 

  while (1) {

    if (Serial2.available()) {      
      uint8_t curChar = Serial2.read();

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
    while (!Serial2.available()) {
      // wait for a character to become available
    }    
    frame[i] = Serial2.read();

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

  Serial.print(distance);
  Serial.print("\t");
  Serial.print(strength);
  Serial.print("\t");
  Serial.println(curMillis);
}