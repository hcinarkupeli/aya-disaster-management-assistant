# AYA - Disaster Management Assistant

**Accessible alerts, wearable sensing and occupancy monitoring in a student engineering project.**

![AYA hardware prototype](assets/prototype-system.jpg)

AYA (*Afet Yönetim Asistanı*) investigates how a local shaking sensor, a wearable alert unit, doorway counting and a mobile interface can support disaster preparedness and information sharing. Developed at Antalya Science and Art Center (BİLSEM), Türkiye, the project received a **2025 TÜBİTAK 2204-B national-final Encouragement Award in Software**.

This repository presents the documented prototype, recovered source sketches, a 3D enclosure model and a reproducible analysis of the historical classification file. It is an engineering research portfolio, not a certified earthquake-warning or clinical monitoring product.

## Team and contribution

- **Hasan Çınar Küpeli:** student co-developer; embedded software, programming and sensor integration, as recorded in the student's contribution account.
- **Luna Hazal Zafran:** student co-developer, identified alongside Hasan in the project presentation.

The account hosting this repository represents Hasan's contribution to a collaborative project. The 2026 repository maintenance and test tooling are distinguished from the 2025 prototype in [provenance](docs/provenance.md).

## System

| Component | Documented implementation |
|---|---|
| Local alert | BMI160-based motion sensing, RF signalling and DFPlayer audio |
| Wearable | Arduino Nano-class controller, optical pulse sensing, MLX90614 temperature sensing, GPS and vibration alert variants |
| Occupancy | Two ultrasonic sensors and a NodeMCU-based doorway counter |
| Mobile interface | MIT App Inventor screens for preparedness, alerts and emergency-kit tracking |
| Enclosure | Prototype photographs and one recovered STL |

See [architecture](docs/architecture.md), [hardware](hardware/README.md) and the [mobile interface](mobile/README.md).

## Run the reproducible analysis

Requires Python 3.10 or later. From the repository root:

```bash
python -m venv .venv
```

Windows PowerShell:

```powershell
.\.venv\Scripts\python.exe -m pip install -r requirements.txt
.\.venv\Scripts\python.exe analysis/risk_analysis.py --output results/local/risk_analysis.json
.\.venv\Scripts\python.exe -m unittest discover -s tests -v
```

macOS / Linux:

```bash
.venv/bin/python -m pip install -r requirements.txt
.venv/bin/python analysis/risk_analysis.py --output results/local/risk_analysis.json
.venv/bin/python -m unittest discover -s tests -v
```

`analysis/risk_analysis.py` validates the input, audits repeated feature pairs and contradictory labels, then compares a deterministic random forest with a majority-class baseline. Identical feature pairs are kept together in the train/test split. Numeric labels 0, 1 and 2 are preserved without assigning clinical meanings.

The file contains 2,000 rows but only 294 unique pulse-temperature pairs; 51 pairs have conflicting labels. Collection method, consent context and labelling procedure are not established in the recovered materials. Results describe this file only. See [data documentation](data/README.md) and [evidence](docs/evidence.md).

## Repository guide

| Path | Contents |
|---|---|
| `archive/firmware_2025/` | Six recovered sketches with embedded credentials removed |
| `analysis/` | Reproducible data audit and baseline comparison |
| `firmware/logic/` | Hardware-independent maintenance logic with C++ tests |
| `hardware/` | Wiring inventory, actual CAD file and geometry notes |
| `assets/` | Original project hardware photographs and selected app screens |
| `docs/` | Engineering account, contribution, evidence and provenance |
| `tests/` | Python tests and host C++ behavioural tests |
| `results/` | Recorded verification and reproducible analysis output |

The App Inventor `.aia` source, a complete gateway application and a working cloud deployment were not recovered. The diagrams show these boundaries explicitly. Existing archived sketches are not presented as a complete, maintained firmware release. Review [archive notes](archive/README.md) before attempting to reproduce the original hardware.

## Research scope

The project report describes a manually shaken tabletop demonstration with 10 students and one wearable trial participant. It does not establish real-earthquake performance, RF range through rubble, calibrated medical accuracy, or a reduction in injuries or mortality. The MLX90614 reading is an object/surface temperature measurement, not automatically a core body-temperature measurement. Use current official emergency instructions in real emergencies.

## Recognition and rights

The award is confirmed in a TÜBİTAK BİDEB/ARBİS participation record dated 12 February 2026. The administrative record is not redistributed because it contains personal identifiers and a verification code. Project authorship and reuse terms are set out in [RIGHTS.md](RIGHTS.md).
