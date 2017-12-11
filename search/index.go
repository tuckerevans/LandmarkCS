package index

import "os"
import "sort"
import "errors"
import "strings"
import "strconv"

/* TODO

   - Implement Forward Creation
   - Implement Inverted from Forward
   - Switch Indexer.go over to this package

/*********
 * Types *
 *********/

type f_info struct {
	word string;
	in_title bool;
	freq float32;
};

type i_info struct {
	doc string;
	in_title bool;
	freq float32;
};

type f_entry {
	this *f_info;
	next *f_info;
};

type i_entry {
	this *i_info;
	next *i_info;
};

type f_index map[string]f_entry;
type i_index map[string]i_index;

type sortInverted {
	w string;
	root *i_entry;
};


/***************************
 * Forward Index Funcitons *
 ***************************/

func NewForwardEntryStrings(text, title []string) *f_entry, error{

}

/****************************
 * Inverted Index Functions *
 ****************************/

func NewInvertedIndexFromFile(fname string) i_index, error{
	var fd *os.File;
	var br *bufio.Reader;
	var err error;
	var buf []byte;
	var tmp i_info;
	var cur *i_info;
	var index i_index;
	var word string
	var info []string;

	fd, err = os.Open(fname);
	if err != nil {
		return nil, err;
	}

	br, err = bufio.NewReader(fd);
	if err != nil {
		return nil, err;
	}

	for err != io.EOF {
		buf, err = br.ReadBytes('\n');
		if buf[0] != '\t' {
			word = strings.Trim(string(buf));
		} else {
			tmp = i_info{nil, false, 0.0};
			info strings.Field(string(buf));
			tmp.String = info[0];
			tmp.in_title = (info[1] == 1);
			tmp.freq = strconv.ParseFloat(info[2], 32);
			if (index[word] == nil) {
				index[word] = &tmp;
			} else {
				cur = index[word];
				for cur.next != nil {
					cur = cur.next;
				}
				cur.next = &i_entry{this: &tmp, next: nil};
			}
		}
	}

	return index;
}

func NewInvertedFromForward(f f_index) i_index, error {

}

func (x i_index) PrintToFile(fd *os.File) error{
	var i int;
	var cur *i_entry;
	var index []sortInverted;

	index = x.sortIndex();
	
	for i = 0; i < len(index); i++ {
		fmt.Fprintf(fd, "%s\n", index[i].w);
		for cur = index[i].root; cur != nil; cur = cur.next {
			fmt.Fprintf(fd, "\t%s %d %.3f", cur.this.doc, toInt(cur.this.in_title), cur.this.freq);
		}
	}
}

func toInt(t bool) int{
	if t
		return 1;
	return 0;
}

func (unsort i_index) sortIndex() []sortInverted {
	var i int;
	var sorted []sortInverted;

	sorted = make([]sortInverted, len(unsort));

	i = 0;
	for k, v := range unsort {
		sorted[i].w = k;
		sorted[i].root = v;
		i++;
	}

	sort.Slice(sorted, func(i, j int) bool {
		return sorted[i].w < sorted[j].w;
	});
}
