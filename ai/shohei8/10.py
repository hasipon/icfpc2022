import subprocess
import time
import random

subprocess.run(["cargo", "build", "--release", "--target", "x86_64-pc-windows-msvc"])

for n in range(1, 1000):
    subprocess.run(["./target/x86_64-pc-windows-msvc/release/application_a", str(10), 40, 8000])
