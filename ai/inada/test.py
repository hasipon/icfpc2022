import json
import subprocess

with open("testcase.json") as f:
    testcase = json.load(f)

for case in testcase:
    print(case["id"])
    isl_path = "../../solutions/" + case["id"]
    print(case["cost"])
    subprocess.run(
        ["", "-l", "/dev/null"],
        env={"ISL_FILE": ""},
        capture_output=True)
