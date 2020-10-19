// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"math/rand"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct {
	aka      string
	ip       string
	lastcon  string
	isAdmin  bool
	isKicked bool
	online   bool
	ch       client
}

var (
	ircPrompt      = "irc-server > "
	serverTimeZone = "America/Mazatlan"
	entering       = make(chan client) // entering channel
	leaving        = make(chan client) // exiting channel
	messages       = make(chan string) // auxiliar data channel
	users          = make(map[client]*user)
)

func broadcaster() {
	//var clients = make(map[client]data) // all connected clients

	for {
		select {
		case msg := <-messages:
			// obtaining user
			tok := strings.Split(msg, " ")
			for _, cli := range users {
				if cli.online && cli.aka != tok[0] { // sending only to online users except for the author
					cli.ch <- msg
				}
			}
			fmt.Println(msg)

		case cli := <-entering:

			info := users[cli]
			// udpate client data
			info.online = true
			info.ch = cli
			info.lastcon = getTime()

			if info.isAdmin {
				messages <- "[admin] " + info.aka + "has joined to lead the Hive."
			}

		case cli := <-leaving:
			logoff(cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+userHandling

func createUser(aka, ip, lastcon string) *user {
	curr := getUserByAka(aka)
	if curr != nil { // if created return that user
		return curr
	}
	return &user{aka: aka, ip: ip, lastcon: lastcon, isAdmin: false, isKicked: false, online: false}
}

func logoff(cli client) {
	u := users[cli]
	u.online = false
	u.lastcon = getTime()
	close(u.ch)
	messages <- u.aka + "now leaving the hive..."
}

func sendAllUsers(ch client) {
	for _, user := range users {
		if user.online {
			ch <- ircPrompt + user.aka + " -  connected since " + user.lastcon
		}
	}
}

func getUserByAka(aka string) *user {
	for _, user := range users {
		if user.aka == aka {
			return user
		}
	}
	return nil
}

func getUserInfo(usr *user) string {
	if usr != nil {
		var status string
		if usr.online {
			status = ", Status: ONLINE, " + "Connected since: " + usr.lastcon
		} else {
			status = ", Status: OFFLINE, " + "Last seen: " + usr.lastcon
		}
		return ircPrompt + "Aka: " + usr.aka + ", IP: " + usr.ip + status
	}
	return ircPrompt + "User not found"
}

func sendMessage(rec *user, sen string, msg string) string {
	if rec != nil {
		rec.ch <- sen + " (whispers) > " + msg
		return sen + " to " + rec.aka + " > " + msg
	}
	return ircPrompt + "user not found"
}

func kickUser(kicked, admin *user) string {
	if kicked != nil {
		if kicked == admin {
			return ircPrompt + "why would you kick yourself? Just log off! duh."
		}
		kicked.isKicked = true
		kicked.ch <- ircPrompt + "*suddenly feels a kick in the back*"
		leaving <- kicked.ch
		messages <- ircPrompt + kicked.aka + " has been kicked by " + admin.aka
		return ircPrompt + " Chuck Norris' kick like effectuated."
	}
	return ircPrompt + "the user was not found so there were no kicks :C"
}

//!-userHandling

//!+handleConn

// !+userHandleConn

func getTime() string {
	_, err := time.LoadLocation(serverTimeZone)
	if err != nil {
		return ircPrompt + "Failed loading timezone, try again later. "
	}
	return time.Now().Format("2006-01-02 15:04:05")
}

func getuser(conn net.Conn) string {
	buf := make([]byte, 512)
	n, _ := conn.Read(buf)
	return string(buf[:n])
}

func randomSuggestion(aka string) string {
	return aka + strconv.Itoa(rand.New(rand.NewSource(time.Now().UnixNano())).Intn(10000))
}

func isAvailable(aka string) string {
	for _, user := range users {
		if user.aka == aka && user.online {
			return isAvailable(randomSuggestion(aka))
		}
	}
	return aka
}

//!-userHandleConn

func handleConn(conn net.Conn) {

	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	aka := isAvailable(getuser(conn))
	user := createUser(aka, conn.RemoteAddr().String(), getTime())
	user.ch = ch
	users[ch] = user
	user.isKicked = false

	var who string
	if user.isAdmin {
		who = "[admin] " + user.aka
	} else {
		who = user.aka
	}

	// Welcome message
	ch <- ircPrompt + "Welcome to the Ismael's Really Cool (IRC) Chat Server!"

	entering <- ch
	// broadcast all clients the arrival
	messages <- ircPrompt + who + " has joined us in the Hive! Hello there, comrade!"

	if len(users) <= 1 { // first user becomes admin by default
		ch <- ircPrompt + "eres admin\n[https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fi.ytimg.com%2Fvi%2Fe8Cd_VfKj2E%2Fhqdefault.jpg&f=1&nofb=1] "
		user.isAdmin = true
	}

	// reading of all messages
	input := bufio.NewScanner(conn)
	for input.Scan() {
		tok := strings.Split(input.Text(), " ")
		switch tok[0] {
		case "/users":
			sendAllUsers(ch)
		case "/msg":
			if len(tok) < 3 {
				ch <- ircPrompt + "Correct usage: /msg <usename> <message>"
			} else {
				dest := getUserByAka(tok[1])
				if dest != nil {
					ch <- sendMessage(dest, who, strings.Join(tok[2:], " "))
				} else {
					ch <- ircPrompt + "User " + tok[1] + " was not found"
				}
			}
		case "/time":
			ch <- ircPrompt + "Local Time: " + serverTimeZone + getTime()
		case "/user":
			if len(tok) <= 1 {
				ch <- ircPrompt + "Usage: /user <username>"
			} else {
				ch <- getUserInfo(getUserByAka(tok[1]))
			}
		case "/kick":
			if user.isAdmin {
				if len(tok) < 2 {
					ch <- ircPrompt + "Usage: /kick <username>"
				} else {
					fmt.Println(kickUser(getUserByAka(tok[1]), user))
				}
			} else {
				ch <- ircPrompt + " you're not allowed to kick users, brah"
			}
		default:
			if !user.isKicked {
				messages <- who + " > " + input.Text()
			} else {
				leaving <- ch
			}
		}
	}

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	if len(os.Args) < 5 {
		log.Println("Error: Badly used parameters. :$")
		log.Println("Correct usage: ./server -host <address> -port <port>")
		log.Println("   	    or: go run server	.go -user <username> -server <address:port>")

		// finish process
		os.Exit(1)
	}

	host := flag.String("host", "localhost", "Address of the host")
	port := flag.String("port", "42069", "Port of the server")

	flag.Parse()

	listener, err := net.Listen("tcp", *host+":"+*port)
	if err != nil {
		log.Fatal(err)
	}

	// My second name is Ismael xd
	log.Println("Ismael's Really Cool (IRC) Chat Server has benn successfully initiated!")
	log.Printf("Server started at: %s:%s", *host, *port)

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
