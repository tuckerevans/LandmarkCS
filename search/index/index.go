package index

import "fmt"
import "os"
import "io"
import "bufio"
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

type F_info struct {
	Word string;
	In_title bool;
	Freq float64;
};

type I_info struct {
	Doc string;
	In_title bool;
	Freq float64;
};

type F_entry struct{
	This *F_info;
	Next *F_entry;
};

type I_entry struct{
	This *I_info;
	Next *I_entry;
};

type F_index map[string]*F_entry;
type I_index map[string]*I_entry;

type sortInverted struct{
	w string;
	root *I_entry;
};


/***************************
 * Forward Index Funcitons *
 ***************************/

func NewForwardEntryStrings(text, title []string) (*F_entry, error) {
	return nil, errors.New("not implemented");
}

/****************************
 * Inverted Index Functions *
 ****************************/

func new_I_info() *I_info{
	return &I_info{"", false, 0.0};
}

func NewInvertedIndexFromFile(fname string) (I_index, error) {
	var fd *os.File;
	var br *bufio.Reader;
	var err error;
	var buf []byte;
	var tmp *I_info;
	var cur *I_entry;
	var index I_index;
	var word string
	var info []string;

	fd, err = os.Open(fname);
	if err != nil {
		return nil, err;
	}

	br = bufio.NewReader(fd);
	if br == nil {
		return nil, errors.New("Could not initialize reader");
	}

	index = make(I_index);

	for buf, err = br.ReadBytes('\n'); err != io.EOF;  buf, err = br.ReadBytes('\n'){
		tmp = new_I_info();
		if err != nil {
			return nil, err;
		}
		if buf[0] != '\t' {
			word = strings.TrimSpace(string(buf));
		} else {
			info = strings.Fields(string(buf));
			tmp.Doc = info[0];
			tmp.In_title = (info[1] == "1");
			tmp.Freq, _ = strconv.ParseFloat(info[2], 32);
			if (index[word] == nil) {
				index[word] = &I_entry{This: tmp, Next: nil};
			} else {
				cur = index[word];
				for cur.Next != nil {
					cur = cur.Next;
				}
				cur.Next = &I_entry{This: tmp, Next: nil};
			}
		}
	}

	return index, nil;
}

func NewInvertedFromForward(f F_index) (I_index, error) {
 return nil, errors.New("not implemented");

}

func (x I_index) PrintToFile(fd *os.File) error{
	var i int;
	var cur *I_entry;
	var index []sortInverted;

	index = x.sortIndex();
	
	for i = 0; i < len(index); i++ {
		fmt.Fprintf(fd, "%s\n", index[i].w);
		for cur = index[i].root; cur != nil; cur = cur.Next {
			fmt.Fprintf(fd, "\t%s %d %.3f\n", cur.This.Doc, toInt(cur.This.In_title), cur.This.Freq);
		}
	}
	return nil;
}

func toInt(t bool) int{
	if (t){
		return 1;
	}
	return 0;
}

func (unsort I_index) sortIndex() []sortInverted {
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
	
	return sorted
}
