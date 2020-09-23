package main

import (
	"strings"

	"golang.org/x/tour/wc"
)

// WordCount ...
func WordCount(s string) map[string]int {

	daWords := strings.Split(s, " ")

	cntWords := make(map[string]int)

	for i := 0; i < len(daWords); i++ {
		cntWords[daWords[i]]++
	}

	return cntWords

}

func main() {

	wc.Test(WordCount)

}
