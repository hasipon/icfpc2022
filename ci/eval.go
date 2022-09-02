package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"path"
	"strings"
)

type EvalResult struct {
	Cost       int    `json:"cost,omitempty"`
	Similarity int    `json:"similarity,omitempty"`
	Sum        int    `json:"sum,omitempty"`
	Output     string `json:"output,omitempty"`
	Tail       string `json:"tail,omitempty"`
}

func splitLines(s string) []string {
	var lines []string
	sc := bufio.NewScanner(strings.NewReader(s))
	for sc.Scan() {
		lines = append(lines, sc.Text())
	}
	return lines
}

func execEvalV2(probID int, isl string) (*EvalResult, error) {
	result := new(EvalResult)
	f, err := os.CreateTemp("", "eval-*")
	if err != nil {
		return nil, err
	}

	defer os.Remove(f.Name())

	_, err = f.WriteString(isl)
	if err != nil {
		return nil, err
	}

	cmd := exec.Command("npx", "ts-node", "index.ts")
	cmd.Env = append(cmd.Env,
		fmt.Sprintf("ISL_FILE=%s", f.Name()),
		fmt.Sprintf("PROBLEM_ID=%d", probID))
	cmd.Dir = path.Join(RepoRoot, "eval-v2")

	fmt.Println(f.Name(), cmd.Dir)

	out, err := cmd.CombinedOutput()
	lines := splitLines(string(out))
	if err == nil {
		err = json.Unmarshal([]byte(lines[len(lines)-1]), result)
	}
	result.Tail = lines[len(lines)-1]
	result.Output = string(out)
	return result, err
}
