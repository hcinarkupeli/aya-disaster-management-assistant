#pragma once
#include <stdint.h>

// First edge establishes a reference. Return zero until an interval is valid.
class PulseInterval {
 public:
  explicit PulseInterval(int threshold = 550) : threshold_(threshold) {}
  unsigned update(int sample, uint32_t now) {
    if (seen_ && uint32_t(now - last_) > 3000) bpm_ = 0;
    if (sample <= threshold_) { high_ = false; return bpm_; }
    if (high_) return bpm_;
    high_ = true;
    if (!seen_) { seen_ = true; last_ = now; return 0; }
    const uint32_t interval = uint32_t(now - last_);
    if (interval < 250) return bpm_;
    last_ = now;
    bpm_ = interval <= 2000 ? 60000u / interval : 0;
    return bpm_;
  }
 private:
  int threshold_;
  bool high_ = false;
  bool seen_ = false;
  uint32_t last_ = 0;
  unsigned bpm_ = 0;
};
