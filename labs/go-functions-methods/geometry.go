// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

// Point ...
type Point struct{ X, Y float64 }

// Distance | traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.getX()-p.getX(), q.getY()-p.getY())
}

// Distance | same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.getX()-p.getX(), q.getY()-p.getY())
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func generateRandom(min, max float64) float64 {
	return min + rand.Float64()*(max+1-min)
}

func main() {
	// reveive sides param
	sides, _ := strconv.Atoi(os.Args[1])

	if sides <= 2 {
		fmt.Println("Error while entering a valid set of parameters")
		fmt.Println("Usage: geometry << 3 or more sides >>")
	}

	// generates i points and add them to path
	path := make(Path, sides)

	// 1st output message
	println("- Generating a [%d] sides figure", sides)

	for true {
		// setting a seed
		rand.Seed(time.Now().UnixNano())

		valid := false

		for i := 0; i < sides; i++ {
			path[i] = Point{X: generateRandom(-100.0, 100.0), Y: generateRandom(-100.0, 100.0)}
		}

		for i := 0; i < sides; i++ {
			if doIntersect(path[i%sides], path[(1+i)%sides], path[(2+i)%sides], path[(3+i)%sides]) {
				valid = true
				break
			}
		}

		if !valid {
			// 2nd output part
			fmt.Println("Figure's vertices")

			var pLen = len(path)
			var res = ""

			for i := 0; i < pLen; i++ {
				fmt.Printf("\t( %6.2f, %6.2f)\n", path[i].getX(), path[i].getY())

				dist := Distance(path[i%pLen], path[(i+1)%pLen])

				if i == 0 {
					res += fmt.Sprintf("%f", dist)
				} else {
					res += " + " + fmt.Sprintf("%f", dist)
				}
			}
			fmt.Println("Figure's perimeter: ")
			fmt.Printf("\t %s = %0.2f\n", res, path.Distance())
			return
		} else {
			valid = false
		}
	}

	var perimeter float64 = 0
	var pLen = len(path)
	res := ""
	for i := 0; i < pLen; i++ {
		println("( %2.1f, %2.1f)", path[i].getX(), path[i].getY())

		// calculating perimeter
		dist := Distance(path[i%pLen], path[(i+1)%pLen])
		perimeter += dist

		if i == 0 {
			res += fmt.Sprintf("%f", dist)
		} else if (i + 1) < pLen {
			res += " + " + fmt.Sprintf("%f", dist)
		} else if (i + 1) == pLen {
			res += " + " + fmt.Sprintf("%f", dist) + " = "
		}

	}

	// 3rd output part
	println("- Figure's perimeter\n")
	println("%s %f", res, perimeter)

}

// onSegment | checks if point q lies on line segment 'pr'
func onSegment(p, q, r Point) bool {
	if q.getX() <= math.Max(p.getX(), r.getX()) && q.getX() >= math.Min(p.getX(), r.getX()) &&
		q.getY() <= math.Max(p.getY(), r.getY()) && q.getY() >= math.Min(p.getY(), r.getY()) {
		return true
	}
	return false
}

// orientation | returns
// 0 ---> points are colinear
// 1 ---> clockwise
// -1 --> counter-clockwise
func orientation(p, q, r Point) int {
	var val float64 = (q.getY()-p.getY())*(r.getX()-q.getX()) - (q.getX()-p.getX())*(r.getY()-q.getY())

	if val == 0 {
		// colinear
		return 0
	} else if val > 0 {
		// clockwise
		return 1
	} else {
		//counter-clockwise
		return -1
	}

}

func doIntersect(p1, q1, p2, q2 Point) bool {
	var o1 int = orientation(p1, q1, p2)
	var o2 int = orientation(p1, q1, q2)
	var o3 int = orientation(p2, q2, p1)
	var o4 int = orientation(p2, q2, q1)

	// general case
	if o1 != o2 && o3 != o4 {
		return false
	}

	//special cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if o1 == 0 && onSegment(p1, q1, p2) {
		return true
	}

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1
	if o2 == 0 && onSegment(p1, q2, p2) {
		return true
	}

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if o3 == 0 && onSegment(q1, p1, q2) {
		return true
	}

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if o4 == 0 && onSegment(q1, p2, q2) {
		return true
	}

	return false // Doesn't fall in any of the above cases

}

func (p Point) getX() float64 {
	return p.X
}

func (p Point) getY() float64 {
	return p.Y
}

//!-path
