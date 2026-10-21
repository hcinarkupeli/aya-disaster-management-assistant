import importlib.util
from pathlib import Path
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("risk", ROOT / "analysis/risk_analysis.py")
risk = importlib.util.module_from_spec(spec)
spec.loader.exec_module(risk)


class AnalysisTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.frame = risk.load_data(ROOT / "data/historical/risk_labels.csv")

    def load_text(self, text):
        with tempfile.TemporaryDirectory() as folder:
            p = Path(folder) / "data.csv"
            p.write_text(text, encoding="utf-8")
            return risk.load_data(p)

    def test_original_dimensions(self):
        self.assertEqual(self.frame.shape, (2000, 3))

    def test_actual_data_conflicts_are_reported(self):
        a = risk.audit(self.frame)
        self.assertEqual(a["unique_feature_pairs"], 294)
        self.assertEqual(a["conflicting_feature_pairs"], 51)
        self.assertEqual(a["class_counts"], {"0": 1694, "1": 257, "2": 49})

    def test_no_identical_feature_pairs_cross_split(self):
        tr, te, groups = risk.split_groups(self.frame)
        self.assertFalse(set(groups[tr]) & set(groups[te]))
        self.assertEqual(len(tr) + len(te), len(self.frame))

    def test_split_reproducible(self):
        a = risk.split_groups(self.frame)
        b = risk.split_groups(self.frame)
        self.assertTrue((a[0] == b[0]).all())
        self.assertTrue((a[1] == b[1]).all())

    def test_metric_reports_include_all_rows(self):
        r = risk.evaluate(self.frame)
        for m in r["models"].values():
            self.assertEqual(sum(map(sum, m["confusion_matrix"])), r["split"]["test_rows"])
            self.assertTrue(0 <= m["accuracy"] <= 1)
        self.assertIn("majority_baseline", r["models"])

    def test_reject_extra_columns(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru,person\n" + "80,36,0,1\n" * 10)

    def test_reject_missing_values(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru\n" + "80,,0\n" * 10)

    def test_reject_nonfinite_values(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru\n" + "inf,36,0\n" * 10)

    def test_reject_invalid_labels(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru\n" + "80,36,3\n" * 10)

    def test_reject_text_values(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru\n" + "unknown,36,0\n" * 10)

    def test_reject_too_small_file(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru\n80,36,0\n")

    def test_reject_single_class(self):
        with self.assertRaises(ValueError):
            self.load_text("Nabiz,Sicaklik,RiskSkoru\n" + "80,36,0\n" * 10)


if __name__ == "__main__":
    unittest.main()
