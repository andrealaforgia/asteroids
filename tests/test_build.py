"""Check that header edits trigger real incremental recompilation."""

from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix="asteroids-build-test-") as directory:
    checkout = Path(directory) / "asteroids"
    shutil.copytree(
        ROOT,
        checkout,
        ignore=shutil.ignore_patterns(
            ".git", "*.o", "*.d", "*.dSYM", "__pycache__", "asteroids"
        ),
    )
    subprocess.run(["make", "-s", "-j4"], cwd=checkout, check=True)
    assert subprocess.run(["make", "-q"], cwd=checkout).returncode == 0
    (checkout / "game/src/main/game_constants.h").touch()
    assert (
        subprocess.run(["make", "-q"], cwd=checkout).returncode == 1
    ), "Game header edit did not trigger rebuilding"
    subprocess.run(["make", "-s", "-j4"], cwd=checkout, check=True)
    (checkout / "engine/core/math/geometry.h").touch()
    assert (
        subprocess.run(["make", "-q"], cwd=checkout).returncode == 1
    ), "Engine header edit did not trigger rebuilding"
    subprocess.run(["make", "-s", "-j4"], cwd=checkout, check=True)
    assert subprocess.run(["make", "-q"], cwd=checkout).returncode == 0
print("PASS incremental rebuilds after game and engine header edits")
