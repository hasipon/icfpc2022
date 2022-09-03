package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"log"
	"mime/multipart"
	"net/http"
	"os"
)

var client = &http.Client{}

func CallSubmitApi(problemID int, name string, isl string) error {
	requestBody := new(bytes.Buffer)
	writer := multipart.NewWriter(requestBody)
	part, err := writer.CreateFormFile("file", name)
	part.Write([]byte(isl))
	writer.Close()

	url := fmt.Sprintf("https://robovinci.xyz/api/submissions/%d/create", problemID)
	req, err := http.NewRequest("POST", url, requestBody)
	if err != nil {
		return err
	}

	req.Header.Set("Authorization", "Bearer "+os.Getenv("API_TOKEN"))
	req.Header.Set("Content-Type", writer.FormDataContentType())

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
