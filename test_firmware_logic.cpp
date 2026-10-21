#include "../firmware/logic/door_counter.h"
#include "../firmware/logic/pulse_interval.h"
#include "../firmware/logic/motion_confirmation.h"
#include <cassert>
#include <iostream>

int main() {
  unsigned checks = 0;
  auto check = [&](bool ok) { assert(ok); ++checks; };
  DoorCounter d;
  d.update(true, false, 0); d.update(false, true, 100);
  check(d.count() == 1);
  d.update(true, true, 200); check(d.count() == 1);
  d.update(false, false, 250);
  d.update(false, true, 300); d.update(true, false, 400);
  check(d.count() == 0);
  d.update(false, false, 450);
  d.update(false, true, 500); d.update(true, false, 600);
  check(d.count() == 0);
  d.update(false, false, 650);
  d.update(true, true, 700); d.update(false, false, 750);
  check(d.count() == 0);
  d.update(true, false, 800); d.update(false, true, 3000);
  check(d.count() == 0);
  d.update(false, false, 3100);
  d.update(true, false, 3200); d.update(false, true, 3300);
  check(d.count() == 1);
  DoorCounter rollover;
  rollover.update(true, false, UINT32_MAX - 50);
  rollover.update(false, true, 50);
  check(rollover.count() == 1);
  PulseInterval p;
  check(p.update(600, 0) == 0);
  p.update(100, 100); check(p.update(600, 1000) == 60);
  check(p.update(600, 1100) == 60);
  p.update(100, 1150); check(p.update(600, 1200) == 60);
  p.update(100, 1300); check(p.update(600, 2000) == 60);
  check(p.update(100, 6000) == 0);
  PulseInterval wrap;
  wrap.update(600, UINT32_MAX - 499); wrap.update(100, 0);
  check(wrap.update(600, 500) == 60);
  MotionConfirmation m;
  check(!m.update(1.0f)); check(!m.update(1.6f)); check(!m.update(1.7f));
  check(m.update(1.8f)); check(!m.update(1.5f));
  m.update(2.0f); check(!m.update(NAN));
  check(!m.update(2.0f)); check(!m.update(2.0f)); check(m.update(2.0f));
  check(!m.update(2.0f, false));
  std::cout << checks << " behavioural checks passed\n";
}
