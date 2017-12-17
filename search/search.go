/************************************************
 *                 README                       *
 * In order for search/index to be accessible   *
 * you must link this folder (search) into your *
 * GOPATH                                       *
 ************************************************/


package main

import "search/index"
import "os"
import "fmt"
import "sort"
import "flag"
import "strings"

type res struct {
	doc string;
	score float64;
};

func main() {
	var init_index, sIndex index.I_index;
	var tmp, results, root *index.I_entry;
	var tmp_score float64;
	var scores map[string]map[string]float64; // scores[doc][word] == score
	var i,j int;
	var searchBool, perWord, docAdded map[string]bool; //map[doc]bool
	var resultSort []res;
	var err error;
	var fname, s string;
	var search []string;

	flag.StringVar(&fname, "f", "./index.dat", "Index file");
	flag.StringVar(&s, "s", "" , "Search phrase");

	flag.Parse();
	if len(s) == 0 {
		fmt.Printf("Usage: search -s \"search phrase\" [-f index_file]");
		os.Exit(1);
	} else {
		search = strings.Fields(s);
	}

	scores = make(map[string]map[string]float64);
	searchBool = make(map[string]bool);
	perWord = make(map[string]bool);
	docAdded = make(map[string]bool);


	sIndex = make(index.I_index);



	init_index, err = index.NewInvertedIndexFromFile(fname);
	if err != nil {
		panic(err)
	}
	for i = 0; i < len(search); i++ {
		sIndex[search[i]] = init_index[search[i]]
	}

	for _, v := range sIndex {
		for tmp = v; tmp != nil; tmp = tmp.Next {
			searchBool[tmp.This.Doc] = true;
			scores[tmp.This.Doc] = make(map[string]float64);
		}
	}

	for _, v := range sIndex {
		for tmp = v; tmp != nil; tmp = tmp.Next {
			perWord[tmp.This.Doc] = true;
		}
		for d := range searchBool {
			if !perWord[d] {
				searchBool[d] = false;
			}
		}
		perWord = make(map[string]bool);
	}

	for k, v := range sIndex {
		for tmp = v; tmp != nil; tmp = tmp.Next {
			if searchBool[tmp.This.Doc] {
				if tmp.This.In_title {
					tmp_score = 1.0;
				} else {
					tmp_score = 0.0;
				}

				scores[tmp.This.Doc][k] = (0.9 * tmp.This.Freq) + (0.1 * tmp_score);
			}
		}

	}

	i = 0;
	results = &index.I_entry{nil, nil}
	root = &index.I_entry{nil, nil};
	results.Next = root;

	j = 0;

	for _ ,v := range sIndex {
		for tmp = v; tmp != nil; tmp = tmp.Next {
			if (searchBool[tmp.This.Doc]) {
				root.This = tmp.This;
				docAdded[root.This.Doc] = false;
				root.Next = &index.I_entry{nil, nil};
				root = root.Next;
			j++
			}
		}
	}

	resultSort = make([]res, j);

	i = 0;
	for root = results.Next; root.Next != nil; root = root.Next {
		if (!docAdded[root.This.Doc]) {
			j = 0;
			tmp_score = 0;
			for _ ,v := range scores[root.This.Doc] {
				tmp_score += v;
				j++;
			}
			tmp_score /= float64(j);
			resultSort[i] = res{root.This.Doc, tmp_score};
			docAdded[root.This.Doc] = true;
			i++;
		}
	}
	resultSort = resultSort[:i];

		sort.Slice(resultSort, func(i, j int) bool {
			return resultSort[i].score > resultSort[j].score;
		});
	
	fmt.Printf("Results: %d\n", len(resultSort));
	for i = 0; i < len(resultSort); i++ {
		fmt.Printf("\t%d. Doc: %s, Score: %.3f\n", i, resultSort[i].doc, resultSort[i].score);
	}
}
