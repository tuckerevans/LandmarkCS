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

type document struct {
	title []string;
	text []string;
}

var r *regexp.Regexp;

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

	r_doc = newDocument();

	r_doc.text = strings.Fields(sanitize.HTML(text));
	r_doc.title = strings.Fields(sanitize.HTML(t_text));

	return r_doc, nil;
}

func init() {
	log.SetOutput(os.Stderr);
	r, _ = regexp.Compile("><");
}

func main() {
	//	var words map[string]index
	var p_dir string //, fname string;
	var err error;
	var i int;

	var files []os.FileInfo;
	var dir, fd *os.File;
	var dir_info os.FileInfo;
	var dir_mode os.FileMode;

	var doc *document;

	flag.StringVar(&p_dir, "d", "./pages", "pages directory");

	flag.Parse();

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

	for i = 0; i < len(files) && i < 1; i++ {
		fd, err = os.Open(fmt.Sprintf("%s/%s", dir_info.Name(), files[i].Name()));
		if err != nil {
			log.Printf("Error reading %s/%s\n", dir_info.Name(), files[i].Name());
		} else {
			doc, err = parseDoc(fd);
			if err != nil {
				log.Printf("Error parsing %s/%s\n", dir_info.Name(), files[i].Name());
			} else {
				fmt.Println(doc.text);
				fmt.Println(doc.title);
			}
		}
	}

}
