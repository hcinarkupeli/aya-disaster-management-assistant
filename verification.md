# Verification record

Date: 18 October 2026.

## Executed checks

- Python: 12 `unittest` tests passed, covering file structure, missing/nonfinite/nonnumeric input, invalid labels, small/single-class files, reproducible group splitting, conflicting labels and metric accounting.
- C++: 25 behavioural assertions passed with `g++ -std=c++11 -Wall -Wextra -Werror`. Checks include passage direction, release-before-rearming, timeout, count floor, clock rollover, first beat, repeated high pulse signal, short interval rejection, stale readings, consecutive motion samples and invalid sensor values.
- The analysis CLI was executed on the historical file. The recorded output is `risk_analysis.json`.
- Relative Markdown links were checked for target existence. The preserved STL geometry and source hashes were checked.
- Published text was checked against connection strings recovered from the original alarm and occupancy sketches; none were retained. Selected images were visually reviewed.

## Executed environment

Python 3.12.14; NumPy 2.3.5; pandas 2.2.3; scikit-learn 1.8.0; GNU C++ host compiler. The GitHub Actions workflow is supplied but has not been executed on the user's GitHub account.

## Reproduced label-file analysis

The fixed group split produced a majority baseline accuracy of 0.8175 and balanced accuracy of 0.3333. The random forest produced accuracy of 0.9197 and balanced accuracy of 0.7417 on the same held-out split. These are rounded summaries of the JSON output, not medical performance figures. The split and model parameters are explicit in the code.

The conflicting-label audit counts 51 feature pairs with more than one label. Both models are evaluated against the file's supplied labels. No claim is made that those labels are clinically correct.

## Verification boundaries

No Arduino/ESP8266 board-core compilation, flash upload, physical sensor calibration, real RF transmission, phone installation, cloud connection or real-earthquake test was performed in this maintenance run. The archived firmware is historical source with documented constraints, not a newly certified integrated release. Host C++ tests exercise only `firmware/logic/`.

## Repeat

```bash
python -m unittest discover -s tests -v
python analysis/risk_analysis.py --output results/local/risk_analysis.json
g++ -std=c++11 -Wall -Wextra -Werror tests/test_firmware_logic.cpp -o aya_logic
```

Run `./aya_logic` on macOS/Linux or `./aya_logic.exe` in a Windows shell with a compatible C++ compiler. No compiler is required to browse the portfolio or run the Python analysis.
