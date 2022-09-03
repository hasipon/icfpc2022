package main

import (
	"fmt"
	"github.com/jmoiron/sqlx"
	"log"
	"os"
	"path/filepath"
)

var (
	RepoRoot string
)

func connect() {
	dbHost := "localhost"
	if os.Getenv("DB_HOST") != "" {
		dbHost = os.Getenv("DB_HOST")
	}

	dbName := "icfpc2022"
	if os.Getenv("DB_NAME") != "" {
		dbName = os.Getenv("DB_NAME")
	}

	dsn := fmt.Sprintf("icfpc2022:icfpc2022@tcp(%s:3306)/%s?multiStatements=true&parseTime=true&charset=utf8mb4&collation=utf8mb4_bin", dbHost, dbName)
	fmt.Println(dsn)

	db, err := sqlx.Open("mysql", dsn)
	if err != nil {
		panic(err)
	}

	defaultDB.DB = db
}

func initDB() {
	err := defaultDB.Init()
	if err != nil {
		panic(err)
	}
}

func main() {
	RepoRoot = os.Getenv("REPO_ROOT")
	if RepoRoot == "" {
		fmt.Println("REPO_ROOT must be set")
		os.Exit(1)
	}

	connect()

	if os.Getenv("INIT_DB") == "1" {
		initDB()
	}

	InsertSubmissionInDirectory(filepath.Join(RepoRoot, "submissions"))
	InsertSolutionsInDirectory(filepath.Join(RepoRoot, "solutions"))

	batchEvalDB()

	solutions, err := defaultDB.FindUnSubmittedSolutions()
	if err != nil {
		panic(err)
	}
	for _, s := range solutions {
		log.Println(s.ID, "is not submitted")
	}
}
