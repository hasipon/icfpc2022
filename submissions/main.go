package main

import (
	_ "embed"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
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

type Solution struct {
	Id    int    `json:"id"`
	Any   any    `json:"any"`
	Moves string `json:"moves"`
}

func getSubmitJson(id int) *Solution {
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
	return &Solution{
		Id:    sub.ProblemId,
		Any:   nil,
		Moves: string(body),
	}
}

func main() {
	submissions := &Submissions{}
	err := json.Unmarshal(listJson, submissions)
	if err != nil {
		log.Fatalln(err)
	}
	for _, submit := range submissions.Submissions {
		id := submit.Id
		path := fmt.Sprintf("./%d.json", id)
		if _, err := os.Stat(path); err == nil {
			continue
		}
		sol := getSubmitJson(id)
		j, err := json.Marshal(sol)
		if err != nil {
			log.Fatalln(err)
		}
		err = os.WriteFile(path, j, os.ModePerm)
		if err != nil {
			log.Fatalln(err)
		}
	}
}
