package scripts

import (
	"fmt"
	"image/color"
	"strconv"

	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/text"
	"github.com/hajimehoshi/ebiten/v2/examples/resources/fonts"
	"golang.org/x/image/font"
	"golang.org/x/image/font/opentype"
)

type Hud struct {
	g         *Game
	curr_cars int
	totl_cars int
	q         []int
	time      int
}

var (
	mplusNormalFont font.Face
	mplusBigFont    font.Face
)

func CreateHud(game *Game) *Hud {
	h := Hud{
		g:         game,
		curr_cars: 0,
		totl_cars: 0,
	}

	h.q = []int{0, 0, 0, 0}

	tt, err := opentype.Parse(fonts.MPlus1pRegular_ttf)
	if err != nil {
		fmt.Println(err)
	}
	const dpi = 72
	mplusNormalFont, err = opentype.NewFace(tt, &opentype.FaceOptions{
		Size:    24,
		DPI:     dpi,
		Hinting: font.HintingFull,
	})
	if err != nil {
		fmt.Println(err)
	}
	mplusBigFont, err = opentype.NewFace(tt, &opentype.FaceOptions{
		Size:    32,
		DPI:     dpi,
		Hinting: font.HintingFull,
	})

	return &h
}

func (h *Hud) Draw(screen *ebiten.Image) error {
	text.Draw(screen, "Spawned cars: "+strconv.Itoa(h.totl_cars), mplusNormalFont, 20, 30, color.Black)
	text.Draw(screen, "Current cars: "+strconv.Itoa(h.curr_cars), mplusNormalFont, 20, 60, color.Black)
	text.Draw(screen, "Active semaphore: "+strconv.Itoa(h.g.semactual), mplusBigFont, 20, 100, color.Black)

	text.Draw(screen, "Queue Oeste: "+strconv.Itoa(h.q[0]), mplusNormalFont, 20, 150, color.Black)
	text.Draw(screen, "Queue Sur: "+strconv.Itoa(h.q[1]), mplusNormalFont, 20, 180, color.Black)
	text.Draw(screen, "Queue Este: "+strconv.Itoa(h.q[2]), mplusNormalFont, 20, 210, color.Black)
	text.Draw(screen, "Queue Norte : "+strconv.Itoa(h.q[3]), mplusNormalFont, 20, 240, color.Black)

	return nil
}
