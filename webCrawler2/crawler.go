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

func addLinks(doc *goquery.Document, jobs chan link, current link, depth int) {
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
		}
	})
}

func consumer(jobs chan *goquery.Document, done chan bool, links chan link) {
	// do stuff with document
	for {
		select {
		case j := <-jobs:
			fmt.Println(j)
		case <-time.After(time.Second * 1):
			if len(jobs) == 0 && len(links) == 0 {
				done <- true
				return
			} else {
				fmt.Printf("docs: %d, links: %d\n", len(jobs), len(links))
			}
		}
	}
}

func worker(done chan bool, jobs chan link, depth int, docs chan *goquery.Document) {
	for {
		select {
		case j := <-jobs:
			if j.depth < depth {
				doc, err := goquery.NewDocument(j.u.String())
				docs <- doc
				if err != nil {
					log.Print("Error Reading Document: " + j.u.String() + err.Error())
					break
				}

				fmt.Printf("Adding links from %s depth: %d...\n", j.u.String(), j.depth)

				addLinks(doc, jobs, j, j.depth)
			}
		case <-time.After(time.Second * 10):
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

	d, _ = strconv.Atoi(os.Args[2])

	if len(os.Args) == 4 {
		w, _ = strconv.Atoi(os.Args[3])
	} else {
		w = 4
	}

	links := make(chan link, 1024*1024)
	docs := make(chan *goquery.Document, 100)
	done := make(chan bool)

	u, err := url.Parse(os.Args[1])
	if err != nil {
		panic(err)
	}

	if !u.IsAbs() {
		panic("Cannot start with relative url")
	}
	links <- link{u, 0}

	//send first job

	for i := 0; i < w; i++ {
		go worker(done, links, d, docs)
		go consumer(docs, done, links)
	}

	for i := 0; i < w*2; {
		<-done

		fmt.Printf("%d done\n", i)
		i++
	}

	tFile.Close()
	close(done)
	close(links)
	close(docs)
}
