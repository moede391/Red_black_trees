#include "mylib.h"
#include "tree.h"
#include "stdlib.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include "getopt.h"
#include "time.h"
#include <string.h>

int getword(char *s, int limit, FILE *stream) {
    int c;
    char *w = s;
    /*assert(limit > 0 && s != NULL && stream != NULL);*/
    while (!isalnum(c = getc(stream)) && EOF != c);
    if (EOF == c) {
        return EOF;
    } else if (--limit > 0) {
        *w++ = tolower(c);
    }
    while (--limit > 0) {
        if (isalnum(c = getc(stream))) {
            *w++ = tolower(c);
        } else if ('\'' == c) {
            limit++;
        } else {
            break;
        }
    }
    *w = '\0';
    return w - s;
}

void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}

static void print_key(char *key, int freq)
{
    printf("%-4d %s\n", freq, key);
}

int main(int argc, char** argv)
{
	const char *optstring = "c:df:orh";
    char* filename = NULL;
    char* dotfilename = "tree-view.dot";
    int print_dot = 0;
    tree_t type = BST;
    int print_depth = 0;
    char word[256];
	clock_t start, end;
	double getWords, searchTime;
	int unknown_words = 0;
	int count = 0;
	FILE* file;
	FILE* dotfile;
	tree t;

    char option;
    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'c':
                filename = optarg;
                break;
            case 'd':
                print_depth = 1;
                break;
            case 'f':
                dotfilename = optarg;
                break;
            case 'o':
                print_dot = 1;
                break;
            case 'r':
                type = RBT;
                break;
	        case 'b':
		        type = BST;
		        break;
            case 'h':
                /*Send help message*/
                fprintf(stdout, "Arguments:\n-c: Check the spelling of the words in a specified file with the current dictionary.\n-d: Print the depth of the tree.\n-f: Specify a file to output the ‘dot’ graph to.\n-o: Output a ‘dot’ form graph to either the specified file in -f or to ‘output-dot.txt’.\n-r: Use an RBT tree rather than a BST.\n-h: Print this help message.");
                break;
        }
    }

	/*** SET T->TYPE TO TYPE ***/
	/*change_tree_type(t, type);*/
	t = tree_new(type);

    start = clock();

	while (getword(word, sizeof word, stdin) != EOF)
    	{
        	fprintf(stderr, "%s\n", word);
	    	insert(t, word);
		count++;
    	}

	if (print_depth == 1 && filename == NULL) {
	    fprintf(stdout, "Tree depth: %d", tree_depth(t));
	    return EXIT_SUCCESS;
	}

	if (print_dot && filename == NULL) {
        dotfile = fopen(dotfilename, "ab+");
        tree_output_dot(t, dotfile);
	}

	end = clock();
	/* time to get words */
	getWords = (end - start) / ((double)CLOCKS_PER_SEC);

	/* test preorder */
	printf("\nPreorder:\n");
	preorder(t, print_key);
	printf("\n");

	/* test inorder */
	printf("Inorder:\n");
	inorder(t, print_key);
	printf("\n");

	fprintf(stderr, "Deepest node: %d\n", tree_depth(t));

        if (filename != NULL)
	{
		file = fopen(filename, "rb");

		start = clock();

		fprintf(stderr, "Unknown words:\n");

		while (getword(word, sizeof word, file) != EOF)
                {
                        if (search(t, word) == 0)
                        {
                                printf("%s, \n", word);
                                unknown_words++;
                        }
                }

                end = clock();
                /* time to search */
                searchTime = (end - start) / ((double)CLOCKS_PER_SEC);

                fprintf(stderr, "\nFill time: %f\nSearch time %f\nUnknown words = %d\n", getWords, searchTime, unknown_words);
	}

	tree_free(t);
	return EXIT_SUCCESS;
}
