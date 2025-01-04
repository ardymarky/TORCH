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

#ifndef TFMINI_SRC_TFMINI_H_  // NOLINT
#define TFMINI_SRC_TFMINI_H_

#if defined(ARDUINO)
#include "Arduino.h"
#else
#include "core/core.h"
#endif

namespace bfs {

class TFMini {
 public:
  Matek3901() {}
  explicit Matek3901(HardwareSerial *bus) : bus_(bus) {}
  void Config(HardwareSerial *bus) {bus_ = bus;}
  bool Begin();
  bool Read();
  inline uint16_t dist() const {return dist_.i2;}
  inline uint16_t strength() const {return strength_.i2;}
  
 private:
  /* Communication */
  static constexpr int16_t COMM_TIMEOUT_MS_ = 5000;
  static constexpr int32_t BAUD_ = 115200;
  HardwareSerial *bus_;
  elapsedMillis t_ms_;
  /* Data */
  typedef union {uint8_t bytes[2]; uint16_t i2;} union_16;
  union_16 dist_, strength_;
  /* Parser */
  static constexpr uint8_t MATEK_HEADER1_ = 0x59;
  static constexpr uint8_t MATEK_HEADER2_ = 0x59;

  static constexpr uint8_t HEADER1_POS_ = 0;
  static constexpr uint8_t HEADER2_POS_ = 1;
  static constexpr uint8_t TFMINI_CHECKSUM_POS_ = 8;

  /* Driver state parameters*/
  static uint8_t TFMini_FRAME_SIZE = 7;
  uint8_t c_;
  uint8_t state_ = 0;
  uint8_t buf_[TFMini_FRAME_SIZE];
  uint8_t checksum_ = 0;
  uint8_t checksumByte = 0;
};

}  // namespace bfs

#endif  // TFMINI_SRC_TFMINI_H_ NOLINT