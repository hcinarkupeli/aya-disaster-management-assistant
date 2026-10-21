"""Audit the historical AYA label file and evaluate a reproducible baseline."""
from __future__ import annotations

import argparse
import json
from pathlib import Path

import numpy as np
import pandas as pd
from sklearn.dummy import DummyClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, balanced_accuracy_score, classification_report, confusion_matrix
from sklearn.model_selection import GroupShuffleSplit

ROOT = Path(__file__).resolve().parents[1]
FEATURES = ["Nabiz", "Sicaklik"]
TARGET = "RiskSkoru"


def load_data(path: Path) -> pd.DataFrame:
    frame = pd.read_csv(path)
    if list(frame.columns) != FEATURES + [TARGET]:
        raise ValueError("Expected exactly Nabiz,Sicaklik,RiskSkoru in this order.")
    if len(frame) < 10:
        raise ValueError("At least ten rows are required for this analysis.")
    for name in frame.columns:
        frame[name] = pd.to_numeric(frame[name], errors="raise")
    if not np.isfinite(frame.to_numpy(dtype=float)).all():
        raise ValueError("Every value must be finite and present.")
    if not frame[TARGET].isin([0, 1, 2]).all():
        raise ValueError("RiskSkoru must contain only integer labels 0, 1 or 2.")
    if frame[TARGET].nunique() < 2:
        raise ValueError("At least two classes are required.")
    frame[TARGET] = frame[TARGET].astype(int)
    return frame


def audit(frame: pd.DataFrame) -> dict:
    pairs = frame.groupby(FEATURES, sort=True)[TARGET]
    return {
        "rows": len(frame),
        "unique_feature_pairs": pairs.ngroups,
        "rows_beyond_first_per_feature_pair": len(frame) - pairs.ngroups,
        "conflicting_feature_pairs": int((pairs.nunique() > 1).sum()),
        "class_counts": {str(k): int(v) for k, v in frame[TARGET].value_counts().sort_index().items()},
        "feature_ranges": {c: {"min": float(frame[c].min()), "max": float(frame[c].max())} for c in FEATURES},
    }


def split_groups(frame: pd.DataFrame):
    groups = frame.groupby(FEATURES, sort=True).ngroup().to_numpy()
    if len(set(groups)) < 5:
        raise ValueError("At least five distinct feature pairs are required.")
    splitter = GroupShuffleSplit(n_splits=1, test_size=0.2, random_state=42)
    train, test = next(splitter.split(frame[FEATURES], frame[TARGET], groups))
    if set(groups[train]) & set(groups[test]):
        raise RuntimeError("Feature groups overlap between train and test.")
    if frame.iloc[train][TARGET].nunique() < 2:
        raise ValueError("Training split has fewer than two classes.")
    return train, test, groups


def evaluate(frame: pd.DataFrame) -> dict:
    train, test, groups = split_groups(frame)
    x_train, x_test = frame.iloc[train][FEATURES], frame.iloc[test][FEATURES]
    y_train, y_test = frame.iloc[train][TARGET], frame.iloc[test][TARGET]
    output = {
        "scope": "Classification of historical file labels only; not clinical validation.",
        "audit": audit(frame),
        "split": {
            "method": "GroupShuffleSplit by identical pulse-temperature pair",
            "random_state": 42, "test_fraction_of_groups": 0.2,
            "train_rows": len(train), "test_rows": len(test),
            "train_groups": len(set(groups[train])), "test_groups": len(set(groups[test])),
            "overlapping_groups": 0,
            "train_class_counts": {str(k): int(v) for k,v in y_train.value_counts().sort_index().items()},
            "test_class_counts": {str(k): int(v) for k,v in y_test.value_counts().sort_index().items()},
        },
        "models": {},
    }
    models = {
        "majority_baseline": DummyClassifier(strategy="most_frequent"),
        "random_forest": RandomForestClassifier(n_estimators=200, random_state=42, n_jobs=1),
    }
    for name, model in models.items():
        model.fit(x_train, y_train)
        pred = model.predict(x_test)
        output["models"][name] = {
            "accuracy": float(accuracy_score(y_test, pred)),
            "balanced_accuracy": float(balanced_accuracy_score(y_test, pred)),
            "confusion_matrix_labels": [0, 1, 2],
            "confusion_matrix": confusion_matrix(y_test, pred, labels=[0, 1, 2]).tolist(),
            "classification_report": classification_report(y_test, pred, labels=[0, 1, 2], output_dict=True, zero_division=0),
        }
    return output


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", type=Path, default=ROOT / "data/historical/risk_labels.csv")
    parser.add_argument("--output", type=Path, default=ROOT / "results/local/risk_analysis.json")
    args = parser.parse_args()
    try:
        result = evaluate(load_data(args.data))
    except (ValueError, OSError, pd.errors.ParserError) as exc:
        parser.exit(2, f"Input error: {exc}\n")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2, allow_nan=False) + "\n", encoding="utf-8")
    print(json.dumps({"audit": result["audit"], "output": str(args.output)}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
