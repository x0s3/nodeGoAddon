package main

import "C"
import (
	"fmt"
	"math/rand"
	"strings"
	"sync"
	"time"
)

func randomInt(min int, max int) int {
	var bytes int
	bytes = min + rand.Intn(max)
	return int(bytes)
}

func processString(a string, c chan string, waitGroup *sync.WaitGroup) {
	defer waitGroup.Done()
	fmt.Printf("now processing: %s\n", a)
	// different random delays, to show that this is synchronously executed
	time.Sleep(time.Second * time.Duration(randomInt(0, 2)))
	c <- a
}

//export GetBookTitle
func GetBookTitle(input *C.char) *C.char {
	var str strings.Builder

	str.WriteString(C.GoString(input) + ": ")

	a := []string{"Nodejs", "Go", "C++!!"}

	c := make(chan string)
	var waitGroup sync.WaitGroup

	for _, element := range a {
		// add to waitGroup
		waitGroup.Add(1)
		go processString(element, c, &waitGroup)
	}

	// append to existing string builder
	for range a {
		str.WriteString(" " + <-c)
	}

	fmt.Println("Waiting for every Goroutine to be done")
	waitGroup.Wait()

	return C.CString(str.String())
}

func main() {
}
