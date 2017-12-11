package main

import "os"
import "golang.org/x/net/html"
import "log"
import "fmt"
import "github.com/PuerkitoBio/goquery"
import "github.com/kennygrant/sanitize"
import "strings"
import "flag"
import "errors"
import "regexp"

type index struct {
	doc string;
	title bool;
	freq int;
}

type wordList struct {
	this *index
	next *wordList
}

type document struct {
	title []string;
	text []string;
}

var r *regexp.Regexp;
var nonAN *regexp.Regexp;

func newDocument() *document {
	return &document{nil, nil};
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

func parseDoc(fd *os.File) (*document, error) {
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

	r_doc = newDocument();
	r_doc.text = strings.Fields(sanitize.HTML(text));
	r_doc.title = strings.Fields(sanitize.HTML(t_text));

	return r_doc, nil;
}

func printIndex(w map[string]*wordList) string {
return "";
}

func init() {
	log.SetOutput(os.Stderr);
	r, _ = regexp.Compile("><");
	nonAN, _ = regexp.Compile("[<>!@#&()–[{}]:;,?/*'\"]|( and)|( a)|( an)|( and)|( are)|( as)|( at)|( be)|( by)|( for)|( from)|( has)|( he)|( in)|( is)|( it)|( its)|( of)|( on)|( that)|( the)|( to)|( was)|( were)|( will)|( with)")
}

func main() {
	//	var words map[string]index
	var p_dir, w, fname string;
	var err error;
	var i, j int;
	var words map[string]*wordList;
	var cur *wordList;
	var tmp *index;

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
			doc, err = parseDoc(fd);
			if err != nil {
				log.Printf("Error parsing %s/%s\n", dir_info.Name(), files[i].Name());
			} else {
				/* Text */
				for j = 0; j < len(doc.text); j++ {
					w = strings.ToLower(doc.text[j]);

					if words[w] == nil{
						tmp = &index{doc: fname, title: false, freq: 1};
						words[w] = &wordList{this: tmp, next: nil};
					}

					for cur = words[w];cur.next != nil && cur.this.doc != fname; cur = cur.next{}

					if cur.this.doc == fname {
						cur.this.freq++
					} else if cur.next == nil {
						tmp = &index{doc: fname, title: false, freq: 1};
						cur.next = &wordList{this: tmp, next: nil};
					} else {
						panic(fmt.Sprintf("%v", cur));
					}
				}
				/* Title */
				for j = 0; j < len(doc.title); j++ {
					w = strings.ToLower(doc.title[j]);

					if words[w] == nil{
						tmp = &index{doc: fname, title: true, freq: 1};
						words[w] = &wordList{this: tmp, next: nil};
					}

					for cur = words[w];cur.next != nil && cur.this.doc != fname; cur = cur.next{}

					if cur.this.doc == fname {
						cur.this.title = true;
						cur.this.freq++;
					} else if cur.next == nil {
						tmp = &index{doc: fname, title: true, freq: 1};
						cur.next = &wordList{this: tmp, next: nil};
					} else {
						panic(fmt.Sprintf("%v", cur));
					}
				}
			}
		}
		/* TEST PRINT */
		for k,v := range words {
			fmt.Printf("k: %s, doc: %s, title: %v, freq:%d\n",k,v.this.doc,v.this.title,v.this.freq);
		}
	}

}
