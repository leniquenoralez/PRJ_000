package main

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func copy(source *os.File, target *os.File) {
	buffer := make([]byte, 4096)
	for {
		readDataLen, err := source.Read(buffer)
		if err == io.EOF {
			break
		}
		if err != nil {
			fmt.Println(err)
			break
		}
		if err != nil {
			panic(err)
		}
		if readDataLen > 0 {
			writeDataLen, err := target.Write(buffer[:readDataLen])
			if readDataLen != writeDataLen {
				fmt.Printf("error writing to target file")
			}
			check(err)
		}
	}
}

func main() {
	source := os.Args[1]
	target := os.Args[2]

	sourceFile, err := os.Open(source)
	defer sourceFile.Close()
	check(err)

	absPath, err := filepath.Abs(target)
	targetFile, err := os.Create(absPath)
	defer targetFile.Close()
	check(err)

	targetFileInfo, err := targetFile.Stat()
	check(err)

	if targetFileInfo.IsDir() {
		newPath := filepath.Join(target, source)
		targetFile, err := os.Create(newPath)
		copy(sourceFile, targetFile)
		defer targetFile.Close()
		check(err)
	} else {
		copy(sourceFile, targetFile)
	}

}
