package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func main() {

	if len(os.Args) < 2 {
	
		fmt.Println("Wrong usage. Suggested: go run clockWall.go <NewYork=localhost:8080 ...>")
		os.Exit(1)
	
	}

	done := make(chan int)
	
	for _, arg := range os.Args[1:] {
	
		log.Println("Defined port :", arg[strings.LastIndex(arg, "=")+1:])
		conn, err := net.Dial("tcp", arg[strings.LastIndex(arg, "=")+1:])
		if err != nil {
			log.Fatal(err)
		}
		defer conn.Close()
		go mustCopy(os.Stdout, conn)
	
	}

	x := 1
	x = <-done

	log.Println("channel has been closed with a value of: ", x)
	
	close(done)

}

func mustCopy(dst io.Writer, src io.Reader) {

	if _, err := io.Copy(dst, src); err != nil {

		log.Fatal(err)

	}

}