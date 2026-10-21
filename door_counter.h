#pragma once
#include <stdint.h>

// Ordered doorway passage with a timeout and release-before-rearming.
class DoorCounter {
 public:
  explicit DoorCounter(uint32_t timeout_ms = 2000) : timeout_(timeout_ms) {}
  int count() const { return count_; }
  void update(bool first, bool second, uint32_t now) {
    if (state_ == Release) {
      if (!first && !second) state_ = Idle;
      return;
    }
    if (state_ != Idle && uint32_t(now - started_) >= timeout_) {
      state_ = Release;
      return;
    }
    switch (state_) {
      case Idle:
        if (first && second) state_ = Release;
        else if (first) { state_ = Enter; started_ = now; }
        else if (second) { state_ = Exit; started_ = now; }
        break;
      case Enter:
        if (second) { ++count_; state_ = Release; }
        break;
      case Exit:
        if (first) { if (count_ > 0) --count_; state_ = Release; }
        break;
      case Release: break;
    }
  }
 private:
  enum State { Idle, Enter, Exit, Release };
  State state_ = Idle;
  int count_ = 0;
  uint32_t started_ = 0;
  uint32_t timeout_;
};
