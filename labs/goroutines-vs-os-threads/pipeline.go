package main

import (
	"fmt"
	"time"
)

var maxStages int

func main() {

	// communication channels	
	in := make(chan int)
	out := make(chan int)

	go link(in, out, 0, 1000000) // begin the first connection

	start := time.Now() // start the timer

	in <- 1 // first message

	fmt.Println("Maximum number of pipeline stages   : ", maxStages)
	fmt.Println("Time to transit trough the pipeline : ", time.Since(start))
}

func link(in <-chan int, out chan int, i int, conns int) {
	if i <= conns {
		maxStages++
		link(in, out, i + 1, conns)
		out <- <-in
	}
}