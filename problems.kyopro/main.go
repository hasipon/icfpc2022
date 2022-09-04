package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
)

type Block struct {
	BlockId    string `json:"blockId"`
	BottomLeft []int  `json:"bottomLeft"`
	TopRight   []int  `json:"topRight"`
	Color      []int  `json:"color"`
}

type Init struct {
	Blocks []Block `json:"blocks"`
}

func printIntArray(file *os.File, a []int) {
	for i, n := range a {
		if i != 0 {
			fmt.Fprint(file, " ")
		}
		fmt.Fprint(file, n)
	}
	fmt.Fprint(file, "\n")
}

func main() {
	for i := 1; i < 100; i++ {
		png := fmt.Sprintf("../problems/%d.png", i)
		_, err := os.Stat(png)
		if err != nil {
			break
		}
		outFile, err := os.Create(fmt.Sprintf("../problems.kyopro/%d.kyopro", i))
		if err != nil {
			log.Fatalln(err)
		}
		if i >= 26 {
			initJson, err := os.ReadFile(fmt.Sprintf("../problems/%d.initial.json", i))
			if err != nil {
				log.Fatalln(err)
			}
			initObj := Init{}
			err = json.Unmarshal(initJson, &initObj)
			if err != nil {
				log.Fatalln(err)
			}
			fmt.Fprintln(outFile, len(initObj.Blocks))
			for _, b := range initObj.Blocks {
				_, err = fmt.Fprintln(outFile, b.BlockId)
				if err != nil {
					log.Fatalln(err)
				}
				printIntArray(outFile, b.BottomLeft)
				printIntArray(outFile, b.TopRight)
				printIntArray(outFile, b.Color)
			}
		} else {
			_, err = fmt.Fprintln(outFile, "0")
			if err != nil {
				log.Fatalln(err)
			}
		}
		{
			pngJson, err := os.ReadFile(fmt.Sprintf("../problems.json/%d.json", i))
			if err != nil {
				log.Fatalln(err)
			}
			pngObj := make([][]int, 0)
			err = json.Unmarshal(pngJson, &pngObj)
			if err != nil {
				log.Fatalln(err)
			}
			_, err = fmt.Fprintln(outFile, len(pngObj))
			if err != nil {
				log.Fatalln(err)
			}
			for _, c := range pngObj {
				printIntArray(outFile, c)
			}
		}
		if i > 35 {
			pngJson, err := os.ReadFile(fmt.Sprintf("../problems.json/%d.source.json", i))
			if err != nil {
				log.Fatalln(err)
			}
			pngObj := make([][]int, 0)
			err = json.Unmarshal(pngJson, &pngObj)
			if err != nil {
				log.Fatalln(err)
			}
			_, err = fmt.Fprintln(outFile, len(pngObj))
			if err != nil {
				log.Fatalln(err)
			}
			for _, c := range pngObj {
				printIntArray(outFile, c)
			}
		}

		err = outFile.Close()
		if err != nil {
			log.Fatalln(err)
		}
	}

}
