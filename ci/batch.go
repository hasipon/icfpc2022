package main

import (
	"database/sql"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path"
	"strconv"
	"strings"
)

// InsertSolutionsInDirectory solutionsディレクトリにあるファイルたちをDBに登録します
func InsertSolutionsInDirectory(solutionsDir string) {
	dirEntries, err := os.ReadDir(solutionsDir)
	if err != nil {
		log.Println("ReadDir failed", err)
		return
	}

	for _, entry := range dirEntries {
		if entry.IsDir() || strings.HasPrefix(entry.Name(), ".") {
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

		fmt.Println("Inserting", problemID, solutionName)
		_, err = defaultDB.RegisterSolution(solutionName, problemID, string(ans))
		if err != nil {
			if strings.Contains(err.Error(), "Error 1062") {
				// ignore duplicate entry
			} else {
				log.Println("Register Solution failed", err)
			}
		}
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
		}

		err = defaultDB.UpdateSolution(solution)
		if err != nil {
			fmt.Println("UpdateSolutionEvalResult err", err)
			panic(err)
		}
	}
}
