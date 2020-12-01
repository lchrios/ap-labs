package scripts

import (
	"fmt"
	"math/rand"
	"sync"
	"time"

	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/ebitenutil"
)

type Car struct {
	game     *Game
	dir      int
	des      int
	trn      int
	speed    float64
	dis      float64
	xPos     float64
	yPos     float64
	run      bool
	turned   bool
	light    bool
	pass     bool
	img      ebiten.Image
	sem      *Semaphore
	cardinal []string
	vuelta   []string
}

var wg sync.WaitGroup

func CarInit(g *Game, spd float64, d int, ds int, s *Semaphore, i int) *Car {

	rand.Seed(time.Now().UnixNano())
	min := 3
	max := 5

	c := Car{
		game:   g,
		speed:  float64(rand.Intn(max-min) + min),
		dir:    d,
		dis:    0,
		xPos:   50,
		yPos:   325,
		run:    true,
		sem:    s,
		turned: false,
		trn:    ds,
		pass:   false,
	}
	c.cardinal = []string{"Oeste", "Sur", "Este", "Norte"}
	c.vuelta = []string{"", "Derecha", "Frente", "Izquierda"}
	c.des = ((c.sem.position) + c.trn) % 4

	c.light = c.sem.state
	switch d := c.dir; d {
	case 0: // Oeste-Este
		img, _, _ := ebitenutil.NewImageFromFile("imgs/carrito.png", ebiten.FilterDefault)
		c.img = *img
		c.xPos = -240 // giro 250
		c.yPos = 480
	case 1: // Sur-Norte
		img, _, _ := ebitenutil.NewImageFromFile("imgs/carrito_a.png", ebiten.FilterDefault)
		c.img = *img
		c.xPos = 480
		c.yPos = 1110 // giro 250
	case 2: // Este-Oeste
		img, _, _ := ebitenutil.NewImageFromFile("imgs/carrito_i.png", ebiten.FilterDefault)
		c.img = *img
		c.xPos = 1110 // giro 325
		c.yPos = 400
	case 3: // Norte-Sur
		img, _, _ := ebitenutil.NewImageFromFile("imgs/carrito_ab.png", ebiten.FilterDefault)
		c.img = *img
		c.xPos = 400
		c.yPos = -230 // giro 325
	}
	go c.checkSemaphore()
	go c.matchSpeed()

	return &c
}

func (c *Car) checkSemaphore() {
	// Checar semáforo
	for true {
		time.Sleep(time.Duration(50) * time.Millisecond)
		c.light = c.sem.state
		if !c.light {
			if !c.run {
				c.carStart()
			}
		} else {
			if c.run {
				c.carStop()
			}
		}
	}
}

func (c *Car) Update(dTime int) error {

	//Dar Vueltas
	if c.run { // control para parar-avanzar el auto
		// si ha pasado el semáforo en distancia pero no lo ha registrado
		// o sea en cuanto detecte que pase el auto
		// antes de salir del mapa
		if (c.dis >= 530 && c.dis < 1050) && !c.pass {
			c.pass = true
			// lo saca de la lista de espera tras el semaforo
			c.dequeueW()
		} else if c.dis >= 1180 { // ya salio del mapa
			c.dequeue() // despawnealo
		}

		/*
			if está en rango de vuelta && destino es hacia donde se da la vuelta
		*/
		if (c.dis >= 650 && c.dis <= 690) && c.trn == 1 { // vuelta derecha
			c.dir = (c.des + 2) % 4
			fmt.Println(c.dis)
			imgdir := []string{"imgs/carrito.png", "imgs/carrito_a.png", "imgs/carrito_i.png", "imgs/carrito_ab.png"}
			img, _, _ := ebitenutil.NewImageFromFile(imgdir[c.dir], ebiten.FilterDefault)
			c.img = *img
		} else if (c.dis >= 700 && c.dis <= 750) && c.trn == 3 { // vuelta izquierda
			c.dir = (c.des + 2) % 4
			fmt.Println(c.dis)
			imgdir := []string{"imgs/carrito.png", "imgs/carrito_a.png", "imgs/carrito_i.png", "imgs/carrito_ab.png"}
			img, _, _ := ebitenutil.NewImageFromFile(imgdir[c.dir], ebiten.FilterDefault)
			c.img = *img
		}

		switch d := c.dir; d { // cambio de textura segun direccion
		case 0: // carro derecha
			c.xPos += c.speed
			c.dis += c.speed
		case 1: // carro arriba
			c.yPos -= c.speed
			c.dis += c.speed
		case 2: // carro izquierda
			c.xPos -= c.speed
			c.dis += c.speed
		case 3: // carro abajo
			c.yPos += c.speed
			c.dis += c.speed
		}
	}
	return nil
}

func (c *Car) Draw(screen *ebiten.Image) error {

	cDo := &ebiten.DrawImageOptions{}
	cDo.GeoM.Translate(c.xPos, c.yPos)

	screen.DrawImage(&c.img, cDo)

	return nil
}

func (c *Car) matchSpeed() {
	for true {
		time.Sleep(time.Millisecond * 10)
		i := int(c.exitQueuePos())
		if i > 0 { // si hay algun auto
			if c.pass { // pasando el semaforo revisa la velocida dde otros
				cr := c.game.exitQueue[c.des][i-1] // guarda referencia al carro frente a el
				if (c.dis - cr.dis) <= 250 {
					if c.speed > cr.speed { // iguala su velocidad
						c.speed = cr.speed // si es mayor
					}
				}
			} else { // si no ha pasado se fija de la lista de espera
				i := int(c.queuePos())
				if i > 0 {
					cr := c.sem.carsAtLight[i-1] // guarda referencia al carro frente a el
					if (c.dis - cr.dis) <= 250 {
						if c.speed > cr.speed { // iguala su velocidad
							c.speed = cr.speed // si es mayor
						}
					}
				}
			}
		}
	}
}

func (c *Car) queuePos() int {
	for i, cr := range c.sem.carsAtLight {
		if c == cr {
			return i
		}
	}
	return -1
}
func (c *Car) exitQueuePos() int {
	for i, cr := range c.game.exitQueue[c.des] {
		if c == cr {
			return i
		}
	}

	return -1
}
func (c *Car) atPos() bool {
	pos := float64(c.queuePos())
	// comparar distancia recorrida, contra distancia de semaforo segun pos
	// Ej. Posición 2 tiene que estar a = distancia de semaforo - distancia de un carro * pos
	//waitPos := c.dis < (290 - 90*pos)
	dist := (500 - 90*pos)
	if c.dis < dist {
		return false
	} else if c.dis == dist {
		return true
	} else {
		return true
	}
}

func (c *Car) carStop() {
	if !c.atPos() || c.pass {
		c.run = true
	} else {
		c.run = false
	}
}

func (c *Car) carStart() {
	c.run = true
}

func (c *Car) dequeueW() { // dequeue de la lista del semaforo
	time.Sleep(50 * time.Millisecond)

	if len(c.sem.carsAtLight) > 0 {

		c.queue()
		i := c.queuePos()
		// elimina el primer elemento
		c.sem.carsAtLight = append(c.sem.carsAtLight[:i], c.sem.carsAtLight[i+1:]...)
	}
}

func (c *Car) queue() {
	dest := c.des
	c.game.exitQueue[dest] = append(c.game.exitQueue[dest], c)
	c.game.hud.q[c.des]++
}

func (c *Car) dequeue() {
	if len(c.game.exitQueue[c.des]) > 0 {
		i := c.queuePos()
		if i > 0 { // remote ith element
			c.game.exitQueue[c.des] = append(c.game.exitQueue[c.des][:i], c.game.exitQueue[c.des][i+1:]...)
			c.sem.cars = append(c.sem.cars[i:], c.sem.cars[i+1:]...)
		} else { // remove first element
			c.game.exitQueue[c.des] = c.game.exitQueue[c.des][1:]
			c.sem.cars = c.sem.cars[1:]

		}
		c.game.hud.q[c.des]--
		c.game.hud.curr_cars--

	}
}
