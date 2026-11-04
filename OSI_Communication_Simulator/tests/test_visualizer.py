import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "python" / "visualizer.py"

class VisualizerTests(unittest.TestCase):
    def test_visualizer_runs_and_shows_both_directions(self):
        result = subprocess.run(
            [sys.executable, str(SCRIPT), "--message", "Test packet", "--delay", "0"],
            capture_output=True, text=True, check=True
        )
        self.assertIn("ENCAPSULATION", result.stdout)
        self.assertIn("DECAPSULATION", result.stdout)
        self.assertIn("Test packet", result.stdout)
        self.assertIn("VISUALIZATION COMPLETE", result.stdout)

if __name__ == "__main__":
    unittest.main()
