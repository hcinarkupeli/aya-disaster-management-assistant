# Dependency references

Python packages are listed in `requirements.txt`; tested versions are recorded in `results/verification.md`. The analysis uses only local files and needs no account, cloud secret or internet access after installation.

Historical firmware uses third-party Arduino libraries:

- [RadioHead RH_ASK](https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html)
- [VirtualWire](https://www.airspayce.com/mikem/arduino/VirtualWire/) - obsolete library, historical receiver only.
- [DFRobot BMI160](https://github.com/DFRobot/DFRobot_BMI160)
- [DFRobot MLX90614](https://github.com/DFRobot/DFRobot_MLX90614)
- [TinyGPSPlus](https://github.com/mikalhart/TinyGPSPlus)
- [PulseSensor Playground](https://github.com/WorldFamousElectronics/PulseSensorPlayground)
- [DFRobot DFPlayer Mini](https://github.com/DFRobot/DFRobotDFPlayerMini)
- [Firebase ESP Client](https://github.com/mobizt/Firebase-ESP-Client) - deprecated, used by historical cloud sketches.

The dependency repositories are references, not a frozen or hardware-verified toolchain. The RadioHead reference documents its default pins and datagram size; these explain the limitations noted for the original wearable sketch. The original project did not preserve complete board-core and library version metadata.
