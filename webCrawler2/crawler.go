package main

import "crypto/md5"
import "fmt"
import "github.com/PuerkitoBio/goquery"
import "log"
import "net/url"
import "os"
import "strconv"
import "strings"
import "sync"
import "sync/atomic"
import "time"

type link struct {
	u     *url.URL
	depth int
}

var mutex *sync.Mutex
var Prev map[string]bool
var base string
var links_visited uint64 = 0

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

func consume(doc *goquery.Document, url link, worker_id int) {
	f, _ := os.Create(fmt.Sprintf("./pages/%x", md5.Sum([]byte(url.u.String()))))
	s, _ := doc.Html()
	f.Write([]byte(s))
}

func worker(done chan bool, jobs chan link, depth int, id int, total uint64) {
	for {
		x := atomic.LoadUint64(&links_visited)
		if x >= total {
			done <- true
			return
		}

		atomic.AddUint64(&links_visited, 1)
		select {
		case j := <-jobs:
			if j.depth < depth {
				doc, err := goquery.NewDocument(j.u.String())
				docs <- doc
				if err != nil {
					log.Print("Error Reading Document: " + j.u.String() + err.Error())
					break
				}

				fmt.Printf("worker %d Working on %s...\n", id, j.u.String())

				consume(doc, j, id)
				addLinks(doc, jobs, j, j.depth, id)
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

	fi, err := os.Lstat("./pages");
	if err != nil {
		fmt.Printf("INIT ERROR: %s\n", err);
	}
	
	if (fi == nil) {
		os.Mkdir("./pages", 0755);
	} else if (fi.Mode().IsRegular()) {
		panic("pages is not a valid directory\n")
	}

}

func main() {
	var d, w, b int
	var t uint64

	if len(os.Args) < 5 {
		fmt.Printf("usage: crawler url depth max_links workers\n")
		panic("test")
	}

	base = strings.TrimPrefix(os.Args[1], "http://www.")
	base = strings.TrimPrefix(base, "https://www.")
	if base == os.Args[1] {
		panic(base)
	}

	d, _ = strconv.Atoi(os.Args[2])
	b, _ = (strconv.Atoi(os.Args[3]))
	t = uint64(b)
	b, _ = (strconv.Atoi(os.Args[3]))
	t = uint64(b)
	w, _ = strconv.Atoi(os.Args[4])

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
		go worker(done, jobs, d, i, t)
	}

	for i := 0; i < w; {
		select {
		case <-done:
			i++
		case <-time.After(1 * time.Second):
			if len(jobs) == (1024 * 1024) {
				i = w
			}
		}
	}

	close(done)
	close(jobs)
}
