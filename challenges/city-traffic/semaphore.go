package citytraffic

import (
	"time"
)

// wait time struct
type wait struct {
	red   int32
	green int32
	turn  int32
}

var state int32   // -1 = stop | 0 = green front | 1 = green left turn
var ctrLane int32 // 0 = north | 1 = west | 2 = south | 3 = east
var wt wait
var run bool

func createSemaphore(lane int32, green int32, red int32, turn int32) {

	state = -1
	ctrLane = lane
	wt.green = green
	wt.red = red
	wt.turn = turn
	run = false

	go startSem()

}

func startSem() {

	run = true

	for run {

		time.Sleep(time.Duration(wt.red) * time.Second)

		state = 0

		time.Sleep(time.Duration(wt.green) * time.Second)

		state = 1

		time.Sleep(time.Duration(wt.turn) * time.Second)

		state = -1
	}

}

func stopSem() {
	run = false
}
