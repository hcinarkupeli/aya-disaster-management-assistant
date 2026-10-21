# Historical classification file

`historical/risk_labels.csv` is the three-column `dataset.csv` recovered from the `aya-yz` project folder. Numeric values and labels are retained. No participant identifiers, timestamps, GPS locations or names occur in this file.

| Column | Meaning in source code | Provenance limit |
|---|---|---|
| Nabiz | Pulse-rate feature | Measurement and calibration protocol unavailable |
| Sicaklik | Temperature feature | Original precision was whole numbers; measurement site undocumented |
| RiskSkoru | Class label 0, 1 or 2 | Numeric codes only; no validated clinical definitions |

The file is not described as a patient cohort, a certified dataset or newly generated synthetic data. Its collection/generation method was not established. It must not be used to validate clinical claims. The maintained analysis reports class balance, contradictory labels, duplicate-group counts and train/test metrics against a majority-class baseline.

Patient-level or participant-level separation cannot be performed because the file has no participant identifier. Grouping by identical feature pair reduces one source of leakage but cannot replace an independent cohort. Do not add personal health data to a public repository.
