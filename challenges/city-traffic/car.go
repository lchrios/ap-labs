package citytraffic

import (
	rl "github.com/gen2brain/raylib-go/raylib"
)

// lane where the car comes from
var lane int32 // 0 = north | 1 = west | 2 = south | 3 = east
var turn int32 // -1 = left | 0 = front | 1 = right

type Car struct {
	position rl.Vector2
}

func create(laneOrg int32, direction int32) {
	lane = laneOrg
	turn = direction

	go carStart()
}

func carStart() {

}
