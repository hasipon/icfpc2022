package main

import (
	"fmt"
	"log"
	"strconv"
	"strings"
)

var errorFlag bool

type Move struct {
}

type State struct {
	b *Block
}

type Block struct {
	child []*Block
	// [minX, maxX)
	minX, minY, maxX, maxY int
}

func (b *Block) in(x, y int) bool {
	return b.minX <= x && x < b.maxX &&
		b.minY <= y && y < b.maxY
}
func (b *Block) lookUpById(ids []int, pos int) *Block {
	id := ids[pos]
	if len(ids) == pos+1 {
		if id == 1 {
			return b
		} else {
			log.Fatalln("failed")
		}
	}
	return b.child[id].lookUpById(ids, pos+1)
}

func (b *Block) lookUp(x, y int) *Block {
	if len(b.child) == 0 {
		if !b.in(x, y) {
			log.Fatal("invalid")
		}
		return b
	}
	for _, c := range b.child {
		if c.minX <= x && x < c.maxX &&
			c.minY <= y && y < c.maxY {
			return c
		}
	}
	log.Fatalln("not found")
	return nil
}

func (b *Block) lookUpX(x int) []*Block {
	if len(b.child) == 0 {
		if b.minX <= x && x < b.maxX {
			return []*Block{b}
		} else {
			log.Fatalln("error")
			return nil
		}
	}
	var ret []*Block
	for _, c := range b.child {
		if c.minX <= x && x < c.maxX {
			ret = append(ret, c)
		}
	}
	if len(ret) == 0 {
		log.Fatalln("fail")
	}
	return ret
}

func (b *Block) lookUpY(y int) []*Block {
	if len(b.child) == 0 {
		if b.minY <= y && y < b.maxY {
			return []*Block{b}
		} else {
			log.Fatalln("error")
			return nil
		}
	}
	var ret []*Block
	for _, c := range b.child {
		if c.minY <= y && y < c.maxY {
			ret = append(ret, c)
		}
	}
	if len(ret) == 0 {
		log.Fatalln("fail")
	}
	return ret
}

func ParseLine(line string, st *State) *Move {
	if strings.HasPrefix(line, "#") {
		return nil
	}
	terms := strings.Split(line, " ")
	numOfTerms := len(terms)
	cmd := terms[0]
	if cmd == "cut" {
		if numOfTerms == 3 {
			// pcut
		} else if numOfTerms == 4 {
			// lcut
		}
	} else if cmd == "color" && numOfTerms == 3 {
		// color
	} else if cmd == "swap" && numOfTerms == 3 {
		// swap
	} else if cmd == "merge" && numOfTerms == 3 {
		// merge
	} else {
		log.Fatalln("invalid line: ", line)
	}
	return nil
}

// [x, y]
// [0, 1], [1, 1]
// [0, 0], [1, 0]

func parseInt(in string) int {
	x, err := strconv.ParseInt(in, 10, 64)
	if err != nil {
		log.Fatalln(err)
	}
	return int(x)
}

func parseId(in string) []int {
	ids := strings.Split(in, ".")
	var ret []int
	for _, id := range ids {
		ret = append(ret, parseInt(id))
	}
	return ret
}

func (s *State) PointCut(terms []string) {
	ids := parseId(terms[1])
	x := parseInt(terms[2])
	y := parseInt(terms[3])
	pb := s.b.lookUpById(ids, 0)
	if !pb.in(x, y) {
		log.Fatalln("not in")
	}

	// left bottom
	pb.child = append(pb.child, &Block{
		minX: pb.minX,
		minY: pb.minY,
		maxX: x,
		maxY: y,
	})
	// right bottom
	pb.child = append(pb.child, &Block{
		minX: x,
		minY: pb.minY,
		maxX: pb.maxX,
		maxY: y,
	})
	// right top
	pb.child = append(pb.child, &Block{
		minX: x,
		minY: y,
		maxX: pb.maxX,
		maxY: pb.maxY,
	})
	// left top
	pb.child = append(pb.child, &Block{
		minX: pb.minX,
		minY: y,
		maxX: x,
		maxY: pb.maxY,
	})
}

func (st *State) LineCut(terms []string) {
	ids := parseId(terms[1])
	ori := strings.ToLower(terms[2])
	co := parseInt(terms[3])
	if ori != "x" && ori != "y" {
		log.Fatalln("failed")
	}
	if ori == "y" {
		targets := st.b.lookUpY(co)
		for _, t := range targets {
			t.child = append(t.child, &Block{
				minY: t.minY,
				maxY: co,
				minX: t.minX,
				maxX: t.maxX,
			})
			t.child = append(t.child, &Block{
				minY: co,
				maxY: t.maxY,
				minX: t.minX,
				maxX: t.maxX,
			})
		}
	}
	if ori == "x" {
		targets := st.b.lookUpY(co)
		for _, t := range targets {
			t.child = append(t.child, &Block{
				minX: t.minX,
				maxX: co,
				minY: t.minY,
				maxY: t.maxY,
			})
			t.child = append(t.child, &Block{
				minX: co,
				maxX: t.maxY,
				minY: t.minY,
				maxY: t.maxY,
			})
		}
	}
}

func main() {
	fmt.Println("hoge")
}
