import subprocess
import time

subprocess.run(["cargo", "build", "--release", "--target", "x86_64-pc-windows-msvc"])

for n in range(0, 25):
    subprocess.Popen(["./target/x86_64-pc-windows-msvc/release/application_a", str(n + 1)])


#subprocess.run(["./target/x86_64-pc-windows-msvc/release/application_a", "a" + str(99), str(1), "133"])