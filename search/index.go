package main


import "os"
import "log"
import "fmt"
import "github.com/PuerkitoBio/goquery"
import "strings"
import "flag"
import "errors"

type index struct {
	doc string;
	title bool;
	freq int;
}

type document struct{
	title []string;
	text []string;
}


func newDocument() (*document) {
	return &document{nil, nil};
}

func parseDoc(fd *os.File) (*document, error) {
	var err error;
	var text, t_text string;
	var doc *goquery.Document; 
	var body, title *goquery.Selection;
	var r_doc *document;
	
	doc, err = goquery.NewDocumentFromReader(fd);
	if (err != nil) {
		log.Printf("goquery error: %s\n", err);
		return nil, errors.New("Can't create goquery documnt");
	}
	
	//TODO test kennygrant/sanatize instead of goquery
	body = doc.Find("body").Not("style").Not("script");
	title = doc.Find("title");
	
	text = body.Text();
	t_text = title.Text();

	r_doc = newDocument();

	r_doc.text = strings.Fields(text);
	r_doc.title = strings.Fields(t_text);
	if (len(r_doc.text) == 1) {
		log.Printf("not splittin!!!!!!!!!!!\n");
		os.Exit(1);
	}

	return r_doc, nil;
}

func init() {
	log.SetOutput(os.Stderr);
}

func main() {
//	var words map[string]index
	var p_dir string//, fname string;
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
	if (err != nil) {
		log.Printf("Error accessing \"%s\":\t%s\n", p_dir, err);
		os.Exit(1);
	}

	dir_info, err = dir.Stat();
	dir_mode = dir_info.Mode();
	
	if (!dir_mode.IsDir()) {
		log.Printf("\"%s\" is not a directory\n", p_dir);
		os.Exit(1);
	}

	files, err = dir.Readdir(0);
	if (err != nil) {
		log.Printf("Error reading %s\n", p_dir);
		os.Exit(1);
	}

	for i=0; i < len(files) && i < 1; i++ {
		fd, err = os.Open(fmt.Sprintf("%s/%s", dir_info.Name(), files[i].Name()));
		if (err != nil) {
			log.Printf("Error reading %s/%s\n", dir_info.Name(), files[i].Name());
		} else {
			doc, err = parseDoc(fd);
			if (err != nil) {
				log.Printf("Error parsing %s/%s\n", dir_info.Name(), files[i].Name());
			} else {
				fmt.Println(doc.text);
				fmt.Println(doc.title);
			}
		}
	}

}
