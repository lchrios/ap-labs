package scripts

import (
	"math/rand"
	"sync"
	"time"

	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/ebitenutil"
)

type Game struct {
	sem       []*Semaphore
	exitQueue [][]*Car
	playing   bool
	num_cars  int
	carChan   chan int
	hud       *Hud
	dTime     int
	bg        ebiten.Image
	semactual int
}

func NewGame(ncars int) Game {
	g := Game{playing: true, num_cars: ncars, dTime: 0, semactual: 0}
	img, _, _ := ebitenutil.NewImageFromFile("imgs/bg.png", ebiten.FilterDefault)

	var wg sync.WaitGroup

	g.bg = *img
	g.hud = CreateHud(&g)
	g.carChan = make(chan int)
	g.sem = make([]*Semaphore, 4)

	g.exitQueue = [][]*Car{[]*Car{}, []*Car{}, []*Car{}, []*Car{}}

	rand.Seed(time.Now().Unix())

	wg.Add(4) // Esperarpa para que los 4 semaforos se terminen de crear
	for i := 0; i < 4; i++ {
		go SemInit(&g, i, &wg)
	}
	wg.Wait() // Una vez creados los 4 seaforos termina de esperar

	g.sem[0].toggleLight()
	go g.handleLights()

	return g
}

func (g *Game) handleLights() {
	for true {
		time.Sleep(10 * time.Second)

		g.sem[g.semactual].toggleLight()
		g.semactual = (g.semactual + 1) % 4
		time.Sleep(1250 * time.Millisecond)
		g.sem[g.semactual].toggleLight()

	}
}

func (g *Game) Update() error {
	if g.playing {
		g.dTime = (g.dTime + 1) % 20
		for i := 0; i < 4; i++ {
			if err := g.sem[i].Update(g.dTime, 2); err != nil {
				g.carChan <- g.dTime
			}
		}
	}
	return nil
}
func (g *Game) Draw(screen *ebiten.Image) error {
	cDo := &ebiten.DrawImageOptions{}
	cDo.GeoM.Translate(0, 0)
	screen.DrawImage(&g.bg, cDo)
	for i := 0; i < 4; i++ {
		if err := g.sem[i].Draw(screen); err != nil {
			return err
		}
	}

	if err := g.hud.Draw(screen); err != nil {
		return err
	}

	return nil
}
