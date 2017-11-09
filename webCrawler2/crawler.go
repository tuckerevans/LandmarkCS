package main

import "os"
import "fmt"
import "log"
import "time"
import "sync"
import "strconv"
import "strings"
import "net/url"
import "github.com/PuerkitoBio/goquery"

type link struct {
	u     *url.URL
	depth int
}

var mutex *sync.Mutex
var Prev map[string]bool
var tFile *os.File
var base string

func validLink(s string) bool {
	return true
	//return (strings.HasSuffix(s, ".html") || strings.HasSuffix(s, "/") || strings.HasSuffix(s, "\\"))
}

func addLinks(doc *goquery.Document, jobs chan link, current link, depth int, worker_id int) {
	doc.Find("body a").Each(func(index int, item *goquery.Selection) {
		link_s, _ := item.Attr("href")

		d := depth + 1

		u, err := url.Parse(link_s)
		if err != nil {
			panic(err)
		}

		if !u.IsAbs() {
			u = current.u.ResolveReference(u)
		}
		if strings.Contains(u.String(), base) && validLink(u.String()) {
			mutex.Lock()
			if !Prev[u.String()] {
				jobs <- link{u, d}
				Prev[u.String()] = true
			}
			mutex.Unlock()
		} else {
			fmt.Printf("%s Failed: contains: %v 2: %v\n", u.String(), strings.Contains(u.String(), base), validLink(u.String()))
		}
	})
}

func consume(doc *goquery.Document, worker_id int) {
	var title string
	doc.Find("title").Each(func(index int, item *goquery.Selection) {
		title = item.Text()
	})
	fmt.Println(title)
}

func worker(done chan bool, jobs chan link, depth int, id int) {
	for {
		fmt.Printf("%d Waiting... buffered: %d\n", id, len(jobs))
		select {
		case j := <-jobs:
			if j.depth < depth {
				doc, err := goquery.NewDocument(j.u.String())
				if err != nil {
					log.Print("Error Reading Document: " + j.u.String() + err.Error())
					break
				}

				fmt.Printf("worker %d Working on %s depth: %d...\n", id, j.u.String(), j.depth)

				consume(doc, id)
				fmt.Println("Adding Links")
				addLinks(doc, jobs, j, j.depth, id)
				fmt.Println("Done Adding Links")
			}
		case <-time.After(time.Second * 10):
			fmt.Printf("Worker %d done\n", id)
			done <- true
			return
		}
	}
}

func init() {
	mutex = &sync.Mutex{}
	Prev = make(map[string]bool)
	var err error
	tFile, err = os.Create("./test.txt")
	if err != nil {
		panic(err)
	}
}

func main() {
	var d, w int

	if len(os.Args) < 3 {
		fmt.Printf("usage: crawler url depth [workers]\n")
		panic("test")
	}

	base = strings.TrimPrefix(os.Args[1], "http://www.")
	base = strings.TrimPrefix(base, "https://www.")
	if base == os.Args[1] {
		panic(base)
	}
	fmt.Println(base)

	d, _ = strconv.Atoi(os.Args[2])

	if len(os.Args) == 4 {
		w, _ = strconv.Atoi(os.Args[3])
	} else {
		w = 4
	}

	jobs := make(chan link, 1024*1024)
	done := make(chan bool)

	u, err := url.Parse(os.Args[1])
	if err != nil {
		panic(err)
	}

	if !u.IsAbs() {
		panic("Cannot start with relative url")
	}
	fmt.Println(u)
	fmt.Println(u.RawPath)
	fmt.Println(u.String())
	fmt.Println("jobs")
	jobs <- link{u, 0}

	//send first job

	for i := 0; i < w; i++ {
		go worker(done, jobs, d, i)
	}

	for i := 0; i < w; {
		select {
		case <-done:
			fmt.Printf("%d done\n", i)
			i++
		case <-time.After(1 * time.Second):
			if len(jobs) == (1024 * 1024) {
				i = w
			}
		}
	}

	tFile.Close()
	close(done)
	fmt.Println(len(jobs))
	close(jobs)
}
