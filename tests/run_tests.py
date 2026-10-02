"""Exercise gameplay with a controlled clock and tracked allocations."""

import argparse
import importlib.util
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "engine_tests", ROOT / "engine/tests/run_tests.py"
)
engine_tests = importlib.util.module_from_spec(spec)
spec.loader.exec_module(engine_tests)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--sanitize", action="store_true")
    args = parser.parse_args()
    sources = sorted((ROOT / "engine/core").rglob("*.c")) + sorted(
        (ROOT / "game/src").rglob("*.c")
    )
    sources = [p for p in sources if p.name not in ("clock.c", "main.c")]
    sources += [
        ROOT / ("engine/tests/" + name) for name in ("test_allocator.c", "test_clock.c")
    ]
    sources += [ROOT / "tests/test_game.c"]
    includes = [p for p in (ROOT / "game/src").iterdir() if p.is_dir()]
    failures = 0
    with tempfile.TemporaryDirectory(prefix="asteroids-game-tests-") as directory:
        binary = Path(directory) / "tests"
        engine_tests.build(sources, binary, args.sanitize, includes)
        for name in (
            "lifecycle",
            "allocation",
            "destruction",
            "saucer",
            "spawn",
            "mute",
            "simulation",
            "collision",
            "randomness",
            "factories",
            "firing",
            "stall",
            "simultaneous",
        ):
            try:
                result = subprocess.run(
                    [str(binary), name],
                    text=True,
                    capture_output=True,
                    timeout=10,
                    env=engine_tests.runtime_environment(),
                )
                passed = result.returncode == 0
                output = result.stdout + result.stderr
            except subprocess.TimeoutExpired:
                passed, output = False, "Test exceeded 10 seconds"
            print(("PASS " if passed else "FAIL ") + name)
            if not passed:
                failures += 1
                print(output)
    raise SystemExit(bool(failures))


if __name__ == "__main__":
    main()
