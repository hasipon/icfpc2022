package main

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"
	"path/filepath"
	"strconv"
	"strings"
	"time"
)

// InsertSolutionsInDirectory solutionsディレクトリにあるファイルたちをDBに登録します
func InsertSolutionsInDirectory(solutionsDir string) {
	dirEntries, err := os.ReadDir(solutionsDir)
	if err != nil {
		log.Println("ReadDir failed", err)
		return
	}

	for _, entry := range dirEntries {
		if !strings.HasSuffix(entry.Name(), ".isl") {
			continue
		}

		sp := strings.Split(entry.Name(), "-")
		problemID, _ := strconv.Atoi(sp[0])
		solutionName := entry.Name()
		ans, err := ioutil.ReadFile(path.Join(solutionsDir, entry.Name()))
		if err != nil {
			log.Println("Read Solution failed", err)
			continue
		}

		_, err = defaultDB.RegisterSolution(solutionName, problemID, string(ans))
		if err != nil {
			if strings.Contains(err.Error(), "Error 1062") {
				// ignore duplicate entry
			} else {
				log.Println("Register Solution failed", err)
			}
		} else {
			fmt.Println("New solution added:", problemID, solutionName)
		}
	}
}

func InsertSubmissionInDirectory(submissionDir string) {
	listJson, err := ioutil.ReadFile(path.Join(submissionDir, "list.json"))
	if err != nil {
		panic(err)
	}

	var jsonBody struct {
		Submissions []*Submission `json:"submissions,omitempty"`
	}

	err = json.Unmarshal(listJson, &jsonBody)
	if err != nil {
		panic(err)
	}

	submissions := jsonBody.Submissions

	for _, s := range submissions {
		islFilePath := path.Join(submissionDir, fmt.Sprintf("%d.isl", s.ID))
		_, err = os.Stat(islFilePath)
		if err == nil {
			islByte, err := ioutil.ReadFile(islFilePath)
			if err != nil {
				panic(err)
			}

			s.Isl = string(islByte)
		}

		err = defaultDB.ReplaceSubmission(s)
		if err != nil {
			log.Println("ReplaceSubmission err", err)
		}
	}
}

func batchSubmit() {
	solutions, err := defaultDB.FindUnSubmittedSolutions()
	if err != nil {
		panic(err)
	}

	for _, s := range solutions {
		log.Println(s.ID, "is not submitted")

		err = CallSubmitApi(s.ProblemID, s.ID, s.Isl)
		if err != nil {
			log.Println(err)
		}

		time.Sleep(1 * time.Second)
	}
}

// DBから未評価のものを探して評価する
func batchEvalDB() {
	for {
		solution, err := defaultDB.FindNoEvalSolution()
		if err == sql.ErrNoRows {
			log.Println("No rows", err)
			break
		}
		if err != nil {
			log.Println("FindNoEvalSolution err", err)
			break
		}

		result, err := execEvalV2(solution.ProblemID, solution.Isl)
		log.Println("execEvalV2", solution.ID, result, err)

		if err == nil {
			solution.Valid = 1
		} else {
			solution.Valid = 9
			solution.EvalError = err.Error()
		}

		if result != nil {
			solution.EvalOutput = result.Output
			solution.EvalMessage = result.Tail
			solution.IslCost = result.IslCost
			solution.SimCost = result.SimCost
			solution.Cost = result.Cost
			if result.ImagePath != "" {
				p := filepath.Join(RepoRoot, "solutions", solution.ID+".png")
				err = os.Rename(result.ImagePath, p)
				if err != nil {
					log.Println("image file rename failed: ", err)
				}
			}
		}

		err = defaultDB.UpdateSolution(solution)
		if err != nil {
			fmt.Println("UpdateSolutionEvalResult err", err)
			panic(err)
		}
	}
}
