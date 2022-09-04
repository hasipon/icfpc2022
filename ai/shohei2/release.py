import subprocess
import time
import random

subprocess.run(["cargo", "build", "--release", "--target", "x86_64-pc-windows-msvc"])

n = random.randrange(0, 25)
subprocess.Popen(["./target/x86_64-pc-windows-msvc/release/application_a", str(n + 1)])

#subprocess.run(["./target/x86_64-pc-windows-msvc/release/application_a", "18"])