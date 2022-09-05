import subprocess
import time
import random

for n in range(1, 1000):
    subprocess.run(["./target/x86_64-pc-windows-msvc/release/application_a", str(11), str(4), str(18000)])
