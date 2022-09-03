package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"path/filepath"
)

var client = &http.Client{}

func CallSubmitApi(problemID int, islFilePath string) error {
	url := fmt.Sprintf("https://robovinci.xyz/api/submissions/%d/create", problemID)
	req, err := http.NewRequest("POST", url, nil)
	if err != nil {
		return err
	}

	req.Header.Set("Authorization", "Bearer "+os.Getenv("API_TOKEN"))
	{
		body := new(bytes.Buffer)
		writer := multipart.NewWriter(body)
		part, err := writer.CreateFormFile("file", filepath.Base(islFilePath))
		b, err := ioutil.ReadFile(islFilePath)
		if err != nil {
			return err
		}
		_, err = part.Write(b)
		if err != nil {
			return err
		}
	}

	resp, err := client.Do(req)
	if err != nil {
		return err
	}

	defer resp.Body.Close()

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return err
	}

	log.Println(string(body))

	return nil
}
