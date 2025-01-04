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

#include "ercf.h"  // NOLINT
#if defined(ARDUINO)
#include "Arduino.h"
#else
#include "core/core.h"
#endif

namespace bfs {

bool ERCF::Begin() {
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

bool ERCF::Read(){
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
            }
            else {
                state_ = 0;
            }
        }

        // Lidar payload frame
        else if (state_ > HEADER2_POS_){
            buf_[ERCF_BYTE_COUNTER_] = c_;

            ERCF_BYTE_COUNTER_++;
            state_++;
            
            if (ERCF_BYTE_COUNTER_ == ERCF_FRAME_SIZE_)
            {
                state_ = 0;
                ERCF_BYTE_COUNTER_ = 0;

                for (i=0; i<ERCF_FRAME_SIZE_; i++){
                    angle_.byteArray[i] = buf_[i];
                }
                return true;
            }
        }
    }
    return false; 
}

} // namespace bfs