package main

import (
	"fmt"
	"log"
	"os"
	"strconv"

	"github.com/AndreCalderonB/City_Traffic/scripts"
	"github.com/hajimehoshi/ebiten"
)

var gm scripts.Game

func init() {
	if len(os.Args) < 2 { //check the arguments are correct
		fmt.Println("Error. Cars number missing")
		os.Exit(3)
	}
	ncars, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("Error. Only numeric values for cars")
		os.Exit(3)
	}
	gm = scripts.NewGame(ncars) // Nuevo juego con 5 carros
}

type Game struct{}

func (g *Game) Update(screen *ebiten.Image) error {
	if err := gm.Update(); err != nil {
		return err
	}
	return nil
}

func (g *Game) Draw(screen *ebiten.Image) {
	if err := gm.Draw(screen); err != nil {
		fmt.Println(err)
	}
}

func (g *Game) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return 900, 900
}

func main() {

	ebiten.SetWindowSize(900, 900)
	ebiten.SetWindowTitle("Aviación Simulator")

	if err := ebiten.RunGame(&Game{}); err != nil {
		log.Fatal(err)
	}

}
