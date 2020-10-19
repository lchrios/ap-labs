// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"flag"
	"io"
	"log"
	"net"
	"os"
	"sync"
)

var wg sync.WaitGroup

//!+
func main() {

	// Validate parameters
	if len(os.Args) < 5 {
		log.Println("Error: Badly used parameters. :$")
		log.Println("Correct usage: ./client -user <username> -server <address:port>")
		log.Println("           or: go run client.go -user <username> -server <address:port>")

		// finish process
		os.Exit(1)
	}

	// user / server flag obtaining
	user := flag.String("user", "stranger", "Username to login to server chat.")
	server := flag.String("server", "localhost:42069", "Address to connect to server.")
	flag.Parse()

	// Connect through tcp to localhost:8000
	conn, err := net.Dial("tcp", *server)
	if err != nil { // Connection error handling
		log.Fatal(err)
	}

	// Generating goroutines group
	wg.Add(1)

	go func() { // receive all incomming messages
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		wg.Done() // signal the one goroutine has finished
	}()

	io.WriteString(conn, *user) // prompt message

	mustCopy(conn, os.Stdin)
	conn.Close()

	wg.Wait() // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
