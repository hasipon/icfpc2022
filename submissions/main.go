package main

import (
	_ "embed"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strconv"
	"time"
)

//go:embed list.json
var listJson []byte
var client = &http.Client{}

type Submissions struct {
	Submissions []Submission `json:"submissions"`
}

type Submission struct {
	Id        int    `json:"id"`
	ProblemId int    `json:"problem_id"`
	FileUrl   string `json:"file_url"`
}

func getSubmitIsl(id int) []byte {
	url := fmt.Sprintf("https://robovinci.xyz/api/submissions/%d", id)
	req, _ := http.NewRequest("GET", url, nil)
	req.Header.Set("Authorization", "Bearer "+os.Getenv("API_TOKEN"))
	resp, err := client.Do(req)
	if err != nil {
		log.Fatalln(err)
	}
	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
	sub := Submission{}
	err = json.Unmarshal(body, &sub)
	if err != nil {
		log.Println(string(body))
		log.Fatalln(err)
	}
	resp, err = client.Get(sub.FileUrl)
	if err != nil {
		log.Fatalln(err)
	}
	defer resp.Body.Close()

	body, err = ioutil.ReadAll(resp.Body)
	if err != nil {
		log.Fatalln(err)
	}
	return body
}

func main() {
	log.SetFlags(log.LstdFlags | log.Llongfile)

	submissions := &Submissions{}
	err := json.Unmarshal(listJson, submissions)
	if err != nil {
		log.Fatalln(err)
	}
	for i, submit := range submissions.Submissions {
		id := submit.Id
		problemId := submit.ProblemId
		log.Printf("%d/%d", i+1, len(submissions.Submissions))
		path := fmt.Sprintf("./%d.isl", id)

		now := time.Now().Unix()
		nowString := strconv.FormatInt(now, 10)
		solutionPath := fmt.Sprintf("../solutions/%d-submitted-%s.isl", problemId, nowString)

		if _, err := os.Stat(path); err == nil {
			continue
		}
		sol := getSubmitIsl(id)
		err = os.WriteFile(path, sol, os.ModePerm)
		if err != nil {
			log.Fatalln(err)
		}
		err = os.WriteFile(solutionPath, sol, os.ModePerm)
		if err != nil {
			log.Fatalln(err)
		}
		time.Sleep(time.Millisecond * 300)
	}
}
