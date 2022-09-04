import subprocess
import time

subprocess.run(["cargo", "build", "--release", "--target", "x86_64-pc-windows-msvc"])

for n in range(6, 25):
    subprocess.Popen(["./target/x86_64-pc-windows-msvc/release/application_a", str(n + 1)])

#subprocess.run(["./target/x86_64-pc-windows-msvc/release/application_a", "18"])