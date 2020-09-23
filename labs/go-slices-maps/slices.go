package main

import "golang.org/x/tour/pic"

// Pic ...
func Pic(dx, dy int) [][]uint8 {

	daPicture := make([][]uint8, dy)

	for i := 0; i < dy; i++ {
		daPicture[i] = make([]uint8, dx)
		for j := 0; j < dx; j++ {
			daPicture[i][j] = uint8(i ^ j)
		}
	}

	return daPicture

}

func main() {

	pic.Show(Pic)

}
