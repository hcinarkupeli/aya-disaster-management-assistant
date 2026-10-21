# Evidence and evaluation

## Confirmed recognition

A TÜBİTAK BİDEB/ARBİS record dated 12 February 2026 identifies AYA as a 2025/1 2204-B Software project with a national-final Encouragement Award and Hasan as a student project owner. The original administrative record remains private. No other competition prize or patent grant is asserted here.

## Reported prototype demonstration

The 2025 report describes a table-mounted unit activated by manually shaking the table, audio/visual/tactile feedback, a doorway counter and a classroom demonstration with 10 students. One participant wore a wristband. These are report-level observations, not raw independently audited sensor measurements.

The report also gives awareness percentages. Raw participant-level questionnaire responses, denominators for each item and an analysis script were not recovered. Some percentage increments do not correspond to whole participants in a ten-person sample. Those values are therefore not reproduced as verified effect sizes or evidence of causality.

The presentation mentions response-time and RF-reliability figures, but no packet logs, distance, sample sizes or timing protocol were recovered. No numerical hardware performance claim is made in this portfolio. Claims about lives saved, mass deployment, official operational integration, medical accuracy and universal novelty are likewise not supported by the recovered evidence.

## Reproducible file analysis

The historical CSV contains only pulse, temperature and a numeric label. No clinical data source or labelling protocol was documented in the recovered files. A random forest can be evaluated against the supplied labels, but this cannot establish medical validity. Repeated feature pairs are grouped before splitting to prevent identical input pairs leaking across train and test sets. Conflicting labels are retained and counted rather than silently corrected.

See `results/risk_analysis.json` for the recorded run and `results/verification.md` for software verification scope. A software test pass is not a device, clinical, disaster-response or cloud-deployment certification.
