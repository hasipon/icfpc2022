package main

import (
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"time"

	_ "github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
)

var defaultDB DB

type DB struct {
	*sqlx.DB
}

const schema = `
CREATE TABLE IF NOT EXISTS solution
(
    id           VARCHAR(255),
    problem_id   INT NOT NULL,
    isl          MEDIUMTEXT NOT NULL,
    hash         VARCHAR(255),
    valid        INT DEFAULT 0,
    cost         INT DEFAULT 0,
    isl_cost     INT DEFAULT 0,
    sim_cost     INT DEFAULT 0,
    eval_message MEDIUMTEXT,
    eval_output  MEDIUMTEXT,
    eval_error   MEDIUMTEXT,
    created_at   DATETIME,
    updated_at   DATETIME,
    PRIMARY KEY (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS submission
(
    id           int,
    problem_id   INT NOT NULL,
    isl          MEDIUMTEXT,
    hash         VARCHAR(255),
    score        INT DEFAULT 0,
    status       VARCHAR(255),
    submitted_at DATETIME
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
`
const indexes = `
CREATE INDEX SOLUTION_VALID ON solution(valid);
CREATE INDEX SOLUTION_COST ON solution(cost);
`

type Solution struct {
	ID          string    `json:"id,omitempty" db:"id"`
	ProblemID   int       `json:"problem_id,omitempty" db:"problem_id"`
	Isl         string    `json:"isl,omitempty" db:"isl"`
	Hash        string    `json:"hash,omitempty" db:"hash"`
	Valid       int       `json:"valid,omitempty" db:"valid"`
	Cost        int       `json:"cost,omitempty" db:"cost"`
	IslCost     int       `json:"isl_cost,omitempty" db:"isl_cost"`
	SimCost     int       `json:"sim_cost,omitempty" db:"sim_cost"`
	EvalMessage string    `json:"eval_message,omitempty" db:"eval_message"`
	EvalOutput  string    `json:"eval_output,omitempty" db:"eval_output"`
	EvalError   string    `json:"eval_error,omitempty" db:"eval_error"`
	CreatedAt   time.Time `json:"created_at" db:"created_at"`
	UpdatedAt   time.Time `json:"updated_at" db:"updated_at"`
}

type Submission struct {
	ID          int       `json:"id,omitempty" db:"id"`
	ProblemID   int       `json:"problem_id,omitempty" db:"problem_id"`
	Isl         string    `json:"isl,omitempty" db:"isl"`
	Hash        string    `json:"hash,omitempty" db:"hash"`
	Score       int       `json:"score,omitempty" db:"score"`
	Status      string    `json:"status,omitempty" db:"status"`
	SubmittedAt time.Time `json:"submitted_at" db:"submitted_at"`
}

func (db DB) Ok() bool {
	return db.DB != nil
}

func (db DB) Init() error {
	_, err := db.Exec(schema + indexes)
	return err
}

func solutionHash(problemID int, isl string) string {
	h := sha1.New()
	h.Write([]byte(isl))
	return fmt.Sprintf("%d-%s", problemID, hex.EncodeToString(h.Sum(nil)))
}

func (db DB) RegisterSolution(name string, problemID int, isl string) (*Solution, error) {
	now := time.Now()

	solution := &Solution{
		ID:        name,
		ProblemID: problemID,
		Isl:       isl,
		Hash:      solutionHash(problemID, isl),
		CreatedAt: now,
		UpdatedAt: now,
	}
	_, err := db.NamedExec(`
INSERT INTO solution (
    id,
    problem_id,
    isl,
    eval_message,
    eval_output,
    eval_error,
    created_at,
    updated_at
) VALUES (
    :id,
    :problem_id,
    :isl,
    :eval_message,
    :eval_output,
    :eval_error,
    :created_at,
    :updated_at
)`, solution)
	if err != nil {
		return nil, err
	}
	return solution, nil
}

func (db DB) FindNoEvalSolution() (*Solution, error) {
	s := new(Solution)
	err := db.QueryRowx("SELECT * FROM solution WHERE valid = 0 ORDER BY RAND() LIMIT 1").StructScan(s)
	return s, err
}

func (db DB) UpdateSolution(solution *Solution) error {
	solution.UpdatedAt = time.Now()
	_, err := db.NamedExec(`
UPDATE solution
SET
	valid = :valid,
	cost = :cost,
	isl_cost = :isl_cost,
	sim_cost = :sim_cost,
	eval_message = :eval_message,
	eval_output = :eval_output,
	eval_error = :eval_error,
	updated_at = :updated_at
WHERE id = :id`, solution)
	return err
}

func (db DB) FindBestSolution(problemID int) (*Solution, error) {
	solution := new(Solution)
	err := db.QueryRowx(`
SELECT * FROM solution WHERE
	problem_id = ? AND valid = 1
ORDER BY cost LIMIT 1`,
		problemID).StructScan(solution)
	return solution, err
}

func (db DB) ReplaceSubmission(submission *Submission) error {
	submission.Hash = solutionHash(submission.ProblemID, submission.Isl)
	_, err := db.NamedExec(`REPLACE INTO submission (
    id,
    problem_id,
    isl,
    hash,
    score,
    status,
    submitted_at
) VALUES (
    :id,
    :problem_id,
    :isl,
    :hash,
    :score,
    :status,
    :submitted_at
)`, submission)
	return err
}
