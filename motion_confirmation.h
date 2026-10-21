#pragma once
#include <math.h>
#include <stdint.h>

// Demonstration threshold; no seismological calibration is implied.
class MotionConfirmation {
 public:
  bool update(float magnitude_g, bool valid = true) {
    if (!valid || !isfinite(magnitude_g) || magnitude_g <= 1.5f) {
      consecutive_ = 0;
      return false;
    }
    if (consecutive_ < 3) ++consecutive_;
    return consecutive_ >= 3;
  }
 private:
  uint8_t consecutive_ = 0;
};
