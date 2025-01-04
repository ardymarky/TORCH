/*
* Arden Markin
* amarkin@crimson.ua.edu
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "tfmini.h"  // NOLINT
#if defined(ARDUINO)
#include "Arduino.h"
#else
#include "core/core.h"
#endif

namespace bfs {

bool TFMini::Begin() {
  bus_->end();
  bus_->begin(BAUD_);
  bus_->flush();
  t_ms_ = 0;
  while (t_ms_ < COMM_TIMEOUT_MS_) {
    if (Read()) {
      return true;
    }
  }
  return false;
}

bool TFMini::Read(){
    while (bus_->available()) {
        c_ = bus_->read();
        if (state_ == HEADER1_POS_) {
        if (c_ == MATEK_HEADER1_) {
            state_++;
        }
        else {
            state_ = 0;
        }
        }

        else if (state_ == HEADER2_POS_) {
        if (c_ == MATEK_HEADER2_) {
            state_++; 
            checksum = 0x59 + 0x59;
        }
        else {
            state_ = 0;
        }
        }

        // Lidar payload frame
        else if (state_ > HEADER2_POS_ && state_ < TFMINI_CHECKSUM_POS_){
            buf_[state_ - HEADER2_POS_ - 1] = c_;

            if (state < TFMini_FRAME_SIZE - 2){checksum += c_;}
            state_++;
        }
        // Lidar checksum
        else if (state_ == TFMINI_CHECKSUM_POS_){
            checksumByte = c_;

            if (checksum != checksumByte) {
                state = 100;
                distance = -1;
                strength = -1;
                Serial.println("ERROR: bad checksum");
                return -1;
            }

            dist_[0] = frame[0];
            dist_[1] = frame[1];
            strength_[0] = frame[2];
            strength_[1] = frame[3];

            state_ = 0;
        }
    }
    return false; 
}

} // namespace bfs