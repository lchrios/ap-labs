package main

import (
	"fmt"
	"time"
)

func main() {
	// channels
	in := make(chan int)
	out := make(chan int)
	commsTotal := 0 
	cps := make([]int, 10) 

	// linkning channels bothways
	go link(in, out)
	go link(out, in)

	// waits for one second periods 5 times
	for i := 0; i < 5; i++ {
		out <- 0
		time.Sleep(time.Duration(1) * time.Second)
		x := <-in
		cps[i] = x
		commsTotal += x

	}

	fmt.Println("Communications Per Second : ", commsTotal, "/", 5, "=",commsTotal/5)
	fmt.Println("Each second: ", cps)
}


// constant communication between channels
func link(in chan int, out chan int) {
	for {
		out <- (1 + <-in)
	}
}
