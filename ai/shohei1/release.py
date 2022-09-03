import subprocess
import time

subprocess.run(["cargo", "build", "--release", "--target", "x86_64-pc-windows-msvc"])

for n in range(0, 3):
    subprocess.Popen(["./target/x86_64-pc-windows-msvc/release/application_a", "18", "initial/18.isl"])


#subprocess.run(["./target/x86_64-pc-windows-msvc/release/application_a", "a" + str(99), str(1), "133"])