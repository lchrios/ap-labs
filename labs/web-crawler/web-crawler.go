// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"gopl.io/ch5/links"
)

//!+sema

// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

// Nodo is a struct which helps to maintain a register about url and its respective depth
type Nodo struct {
	url   string
	depth int
}

func crawl(nodo Nodo, fileName string, depth int) []Nodo {

	// open file
	file, err := os.OpenFile(fileName, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)

	if err != nil {
		log.Println(err)
	}

	// write url in file
	if _, err := file.WriteString(nodo.url + "\n"); err != nil {
		log.Println(err)
	}

	fmt.Println(nodo.url)
	file.Close()

	// check if is at bottom line
	if nodo.depth < depth {
		tokens <- struct{}{} // acquire a token

		// obtains all links assosiated to the url
		list, err := links.Extract(nodo.url)

		// declare all new nodos to be crawled at the next depth level
		temp := make([]nodo, len(list))
		for i, url := range list {
			temp[i] = nodo{url: url, depth: nodo.depth + 1}
		}

		<-tokens // release the token

		if err != nil {
			log.Print(err)
		}

		return temp
	}

	return []nodo{}
}

//!-sema

//!+
func main() {

	if len(os.Args) != 4 {
		fmt.Println("Error: wrong parameter input.\nCorrect usage: ./web-crawler -depth=<depth> -results=<results file> <url>")
		os.Exit(1)
	}

	worklist := make(chan []nodo) // where nodes will be sent to be crawled
	var n int                     // pending sends to worklist

	initURL := os.Args[3:]
	dp := flag.Int("depth", 1, "Depth")
	fl := flag.String("results", "results.txt", "Result file")
	flag.Parse()

	urls := make([]nodo, len(initURL))

	for i, url := range initURL {
		urls[i] = nodo{url: url, depth: 0}
	}

	// Start with the command-line arguments.
	n++
	go func() { worklist <- urls }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link.url] {
				seen[link.url] = true
				n++
				go func(link nodo) {
					worklist <- crawl(link, *fl, *dp)
				}(link)
			}
		}
	}

}

//!-
