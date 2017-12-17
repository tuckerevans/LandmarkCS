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
	length int;
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

var r, nonAN *regexp.Regexp;
var stopWords []*regexp.Regexp;


func newDocument() *document {
	return &document{"" , nil, nil, 0};
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
	var i int;

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
	t_text = r.ReplaceAllString(t_text, "> <");

	text = sanitize.HTML(text);
	t_text = sanitize.HTML(t_text);

	text = strings.ToLower(text);
	t_text = strings.ToLower(t_text);

	text = nonAN.ReplaceAllString(text, " ");
	t_text = nonAN.ReplaceAllString(t_text, " ");


	for i = 0; i < len(stopWords); i++ {
		text = stopWords[i].ReplaceAllString(text, " ");
		t_text = stopWords[i].ReplaceAllString(t_text, " ");
	}
	r_doc = newDocument();

	r_doc.fname = f_info.Name();
	r_doc.text = strings.Fields(text);
	r_doc.title = strings.Fields(t_text);
	r_doc.length = len(r_doc.text) + len(r_doc.title);
	fmt.Println(r_doc.length)

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
			freq = float64(cur.this.freq) / float64(cur.this.doc.length);
			
			fmt.Fprintf(fd,"\t%s %d %.3f\n", fname, t, freq);
		}
	}
}

func init() {
	var err error;
	log.SetOutput(os.Stderr);
	r, err = regexp.Compile("><");
	if err != nil {
		panic(err);
	}
	nonAN, err = regexp.Compile("[^a-zA-Z0-9]+");
	if err != nil {
		panic(err);
	}
	//TODO add func to read in stop words from a file;
	stopWords = make([]*regexp.Regexp, 26)
	if err != nil {
		panic(err);
	}
	stopWords[0], err = regexp.Compile("\\W+and\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[1], err = regexp.Compile("\\W+a\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[2], err = regexp.Compile("\\W+an\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[3], err = regexp.Compile("\\W+and\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[4], err = regexp.Compile("\\W+are\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[5], err = regexp.Compile("\\W+as\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[6], err = regexp.Compile("\\W+at\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[7], err = regexp.Compile("\\W+be\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[8], err = regexp.Compile("\\W+by\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[9], err = regexp.Compile("\\W+for\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[10], err = regexp.Compile("\\W+from\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[11], err = regexp.Compile("\\W+has\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[12], err = regexp.Compile("\\W+he\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[13], err = regexp.Compile("\\W+in\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[14], err = regexp.Compile("\\W+is\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[15], err = regexp.Compile("\\W+it\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[16], err = regexp.Compile("\\W+its\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[17], err = regexp.Compile("\\W+of\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[18], err = regexp.Compile("\\W+on\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[19], err = regexp.Compile("\\W+that\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[20], err = regexp.Compile("\\W+the\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[21], err = regexp.Compile("\\W+to\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[22], err = regexp.Compile("\\W+was\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[23], err = regexp.Compile("\\W+were\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[24], err = regexp.Compile("\\W+will\\W+");
	if err != nil {
		panic(err);
	}
	stopWords[25], err = regexp.Compile("\\W+with\\W+");
	if err != nil {
		panic(err);
	}
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
						tmp = &index{doc: doc, title: false, freq: 0};
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
						tmp = &index{doc: doc, title: true, freq: 0};
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
