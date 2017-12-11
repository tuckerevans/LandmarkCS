package main

import "os"
import "sort"
import "golang.org/x/net/html"
import "log"
import "fmt"
import "github.com/PuerkitoBio/goquery"
import "github.com/kennygrant/sanitize"
import "strings"
import "flag"
import "errors"
import "regexp"

type document struct {
	fname string;
	title []string;
	text []string;
}

type index struct {
	doc *document;
	title bool;
	freq int;
}

type wordSort struct {
	w string;
	root *wordList;
}

type wordList struct {
	this *index
	next *wordList
}

var r, nonAN, stopWords *regexp.Regexp;


func newDocument() *document {
	return &document{"" , nil, nil};
}

func RemoveNode(r, rn *html.Node) {
	var found bool;
	var n, item *html.Node;
	var nodes map[int]*html.Node;
	var i, j int;

	found = false;
	nodes = make(map[int]*html.Node);

	for n = r.FirstChild; n != nil; n = n.NextSibling {
		if n == rn {
			found = true;
			n.Parent.RemoveChild(n);
		}

		nodes[i] = n;
		i++;
	}

	if !found {
		for j = 0; j < i; j++ {
			item = nodes[j];
			RemoveNode(item, rn);
		}
	}
}
func RemoveTag(doc *goquery.Selection, tag string) {
	doc.Find(tag).Each(func(i int, s *goquery.Selection) {
		RemoveNode(doc.Get(0), s.Get(0));
	});
}

func logReg(h []byte) []byte {
	log.Printf("RegExp: %s", h);
	return h;
}

func parseDoc(fd *os.File, f_info os.FileInfo) (*document, error) {
	var err error;
	var text, t_text string;
	var doc *goquery.Document;
	var body, title *goquery.Selection;
	var r_doc *document;

	doc, err = goquery.NewDocumentFromReader(fd);
	if err != nil {
		log.Printf("goquery error: %s\n", err);
		return nil, errors.New("Can't create goquery documnt");
	}

	body = doc.Find("body");
	RemoveTag(body, "script");
	RemoveTag(body, "noscript");

	title = doc.Find("title");

	//TODO add error detection
	text, err = body.Html();
	t_text, err = title.Html();


	text = r.ReplaceAllString(text, "> <");
	t_text = r.ReplaceAllString(text, "> <");

	text = sanitize.HTML(text);
	t_text = sanitize.HTML(t_text);

	text = nonAN.ReplaceAllString(text, " ");
	t_text = nonAN.ReplaceAllString(t_text, " ");

	text = stopWords.ReplaceAllString(text, "");
	t_text = stopWords.ReplaceAllString(t_text, "");

	r_doc = newDocument();
	r_doc.fname = f_info.Name();
	r_doc.text = strings.Fields(sanitize.HTML(text));
	r_doc.title = strings.Fields(sanitize.HTML(t_text));

	return r_doc, nil;
}
func boolToInt(t bool) int {
	if t {
		return 1;
	}
	return 0;
}

func printIndex(words []wordSort, fd *os.File) {
	var i int;
	var cur *wordList;
	var fname string;
	var t int;
	var freq float64;

	for i = 0; i < len(words); i++ {
		fmt.Fprintf(fd, "%s\n", words[i].w);
		for cur = words[i].root; cur != nil; cur = cur.next {
			fname = cur.this.doc.fname;
			t = boolToInt(cur.this.title);
			freq = float64(cur.this.freq) / float64(len(cur.this.doc.text));
			
			fmt.Fprintf(fd,"\t%s %d %.3f\n", fname, t, freq);
		}
	}
}

func init() {
	log.SetOutput(os.Stderr);
	r, _ = regexp.Compile("><");
	nonAN, _ = regexp.Compile("[^a-zA-Z0-9]+");
	stopWords, _ = regexp.Compile("( and\\W)|( a\\W)|( an\\W)|( and\\W)|( are\\W)|( as\\W)|( at\\W)|( be\\W)|( by\\W)|( for\\W)|( from\\W)|( has\\W)|( he\\W)|( in\\W)|( is\\W)|( it\\W)|( its\\W)|( of\\W)|( on\\W)|( that\\W)|( the\\W)|( to\\W)|( was\\W)|( were\\W)|( will\\W)|( with\\W)")
}

func main() {
	//	var words map[string]index
	var p_dir, w, fname string;
	var err error;
	var i, j int;
	var words map[string]*wordList;
	var cur *wordList;
	var tmp *index;
	var sorted []wordSort;

	var files []os.FileInfo;
	var dir, fd *os.File;
	var dir_info, fd_info os.FileInfo;
	var dir_mode os.FileMode;

	var doc *document;

	flag.StringVar(&p_dir, "d", "./pages", "pages directory");

	flag.Parse();

	words = make(map[string]*wordList);

	dir, err = os.Open(p_dir);
	if err != nil {
		log.Printf("Error accessing \"%s\":\t%s\n", p_dir, err);
		os.Exit(1);
	}

	dir_info, err = dir.Stat();
	dir_mode = dir_info.Mode();

	if !dir_mode.IsDir() {
		log.Printf("\"%s\" is not a directory\n", p_dir);
		os.Exit(1);
	}

	files, err = dir.Readdir(0);
	if err != nil {
		log.Printf("Error reading %s\n", p_dir);
		os.Exit(1);
	}

	for i = 0; i < len(files); i++ {
		fd, err = os.Open(fmt.Sprintf("%s/%s", dir_info.Name(), files[i].Name()));
		fd_info, err = fd.Stat();
		if err != nil {
			log.Printf("Error getting info\n");
			os.Exit(1);
		}
		fname = fd_info.Name();

		if err != nil {
			log.Printf("Error reading %s/%s\n", dir_info.Name(), files[i].Name());
		} else {
			fmt.Printf("Indexing %s...\n", fname);
			doc, err = parseDoc(fd, fd_info);
			if err != nil {
				log.Printf("Error parsing %s/%s\n", dir_info.Name(), files[i].Name());
			} else {
				/* Text */
				for j = 0; j < len(doc.text); j++ {
					w = strings.ToLower(doc.text[j]);

					if words[w] == nil{
						tmp = &index{doc: doc, title: false, freq: 1};
						words[w] = &wordList{this: tmp, next: nil};
					}

					for cur = words[w];cur.next != nil && cur.this.doc.fname != fname; cur = cur.next{}

					if cur.this.doc.fname == fname {
						cur.this.freq++
					} else if cur.next == nil {
						tmp = &index{doc: doc, title: false, freq: 1};
						cur.next = &wordList{this: tmp, next: nil};
					} else {
						panic(fmt.Sprintf("%v", cur));
					}
				}
				/* Title */
				for j = 0; j < len(doc.title); j++ {
					w = strings.ToLower(doc.title[j]);

					if words[w] == nil{
						tmp = &index{doc: doc, title: true, freq: 1};
						words[w] = &wordList{this: tmp, next: nil};
					}

					for cur = words[w];cur.next != nil && cur.this.doc.fname != fname; cur = cur.next{}

					if cur.this.doc.fname == fname {
						cur.this.title = true;
						cur.this.freq++;
					} else if cur.next == nil {
						tmp = &index{doc: doc, title: true, freq: 1};
						cur.next = &wordList{this: tmp, next: nil};
					} else {
						panic(fmt.Sprintf("%v", cur));
					}
				}
			}
		}
		fd.Close();
	}
		sorted = make([]wordSort, len(words));
		i = 0;
		for k,v := range words {
			sorted[i].w = k;
			sorted[i].root = v;
			i++;
		}
		
		sort.Slice(sorted, func(i, j int) bool {
			return sorted[i].w < sorted[j].w;
		});
		
		fd,_ = os.Create("index.dat");
		printIndex(sorted, fd);
		fd.Close();
}
