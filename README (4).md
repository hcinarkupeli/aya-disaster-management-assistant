# Historical prototype source

These are source sketches recovered from the 2025 project folders. They are separate hardware variants, not six programs to run together. Sketch folder names match their `.ino` filenames for Arduino IDE use. The source originally named `deprem_scklk.ino` actually implements temperature, pulse and GPS functions; it is filed as `wristband_temperature_gps` here.

Embedded network credentials have been removed. The two historical Firebase sketches require a locally populated `secrets.h`; a configuration example is supplied. Empty strings are intentionally unset configuration, not working service credentials. The referenced Firebase-ESP-Client library is deprecated; these sketches are preserved for research provenance, not represented as a supported cloud deployment.

The original partial three-reading alarm fragment is not a standalone sketch and is not included as a program. A broken CSV-conversion utility that expected a different five-column schema is also excluded. The maintained Python analysis uses the actual three-column file directly.

## Observed implementation constraints

| Sketch | Constraint identified by source inspection |
|---|---|
| alarm | Single-axis threshold, blocking audio/RF loops and blocking network connection. Published design is local shaking detection, not earthquake prediction. |
| occupancy | Blocking doorway waits, no pulse timeout, possible negative count, and a send timer that is not advanced. A non-passing object can stall the loop. |
| wristband_rf | RadioHead default TX pin 12 overlaps the motor pin; historical text payload may exceed the radio payload budget. AVR `snprintf` does not normally format floats. |
| receiver_virtualwire | Unbounded motor-on response to any message; first pulse interval is not a validated beat interval. |
| wristband_gps | Pulse processing starts only after a GPS fix, and GPS parsing stops afterwards. |
| wristband_temperature_gps | Location flag starts true, so the GPS-reading branch is skipped. |

These notes preserve the distinction between the historical implementation and later maintenance work. Host-testable maintenance logic in `firmware/logic/` addresses bounded counting, pulse intervals and consecutive-threshold detection; it is separate from the archived wiring and has not been flashed to a device.
