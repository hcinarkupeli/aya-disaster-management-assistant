# Hardware inventory

| Unit | Components described in the source | Source pin assignments |
|---|---|---|
| Local alert | NodeMCU/ESP8266, BMI160, 433 MHz transmitter, DFPlayer Mini | RF TX D3; DFPlayer software serial RX D5 / TX D6; BMI160 I2C address 0x69 |
| Wearable RF | Arduino Nano-class board, MLX90614, analog pulse sensor, GPS, RF, vibration driver | GPS RX 3 / TX 4; pulse A0; motor 12; historical RadioHead uses default pins, with a conflict on pin 12 |
| Wearable receiver | Analog pulse input, MLX90614, VirtualWire receiver, vibration driver | RF RX 6; pulse A0; motor 12 |
| Occupancy | NodeMCU/ESP8266 and two ultrasonic sensors | Sensor 1 trigger D5 / echo D6; sensor 2 trigger D2 / echo D3 |

These assignments document archived code, not a validated production schematic. Verify the actual board and revision before wiring. ESP8266 GPIO is 3.3 V: use an appropriate level interface for 5 V ultrasonic echo outputs. Drive a vibration motor through a suitable transistor/driver with flyback protection, not directly from a GPIO. I2C pins follow the board configuration. GPIO0/D3 is a boot-strapping pin on ESP8266; attached circuitry must not force an invalid boot level.

The original source variants use different RF pins and libraries; do not combine their wiring blindly. RF compatibility, timing, motor drive and power supply must be checked on hardware.

## Original prototype photographs

![Alarm hardware](../assets/alarm-hardware.jpg)
![Wearable electronics](../assets/wristband-electronics.jpg)
![Wearable enclosure](../assets/wristband-enclosure.jpg)
![Doorway sensor bar](../assets/occupancy-sensor-bar.jpg)

## CAD

`cad/ceiling_enclosure.stl` is the recovered `Tavan kutusu.stl`, unchanged. It contains 6,436 triangles. Its coordinate extents are approximately 84.000 × 62.980 × 36.000 model units. STL does not encode units; millimetres must be confirmed in the original design/slicer, not inferred as a guaranteed fabrication size. A complete set of wearable and doorway enclosure meshes was not recovered.
