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

#ifndef ERCF_SRC_ERCF_H_  // NOLINT
#define ERCF_SRC_ERCF_H_

#if defined(ARDUINO)
#include "Arduino.h"
#else
#include "core/core.h"
#endif

namespace bfs {

class ERCF {
 public:
  ERCF() {}
  explicit ERCF(HardwareSerial *bus) : bus_(bus) {}
  void Config(HardwareSerial *bus) {bus_ = bus;}
  bool Begin();
  bool Read();
  inline float angle() const {return angle_.angle_uart;}
  
 private:
  /* Communication */
  static constexpr int16_t COMM_TIMEOUT_MS_ = 5000;
  static constexpr int32_t BAUD_ = 115200;
  HardwareSerial *bus_;
  elapsedMillis t_ms_;
  /* Data */
  typedef union {float angle_uart; byte byteArray[4];} union_float;
  union_float angle_;
  /* Parser */
  static constexpr uint8_t ERCF_HEADER1_ = 0x69;
  static constexpr uint8_t ERCF_HEADER2_ = 0x69;

  static constexpr uint8_t HEADER1_POS_ = 0;
  static constexpr uint8_t HEADER2_POS_ = 1;

  /* Driver state parameters*/
  static uint8_t ERCF_FRAME_SIZE_ = 4;
  static uint8_t ERCF_BYTE_COUNTER_ = 0;
  uint8_t c_;
  uint8_t state_ = 0;
  uint8_t buf_[ERCF_FRAME_SIZE];

};

}  // namespace bfs

#endif  // ERCF_SRC_ERCF_H_ NOLINT