#include "list.h"
#include "rule.h"
#include "match.h"
#include "tree.h"
#include "input.h"
#include "pretty.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include <time.h>

extern Expression* head_left;
extern Expression* head_right;

extern NTree* root;
extern Rule *rules[];

int main(int argc, const char * argv[]) {

	clock_t begin = clock();

	if (argc < 7) {
		printf("Too few arguments %d for unify; expected 6\n", argc-1);
		exit(-1);
	}
	else if (argc > 7) {
    	printf("Too many arguments %d for unify; expected 6\n", argc-1);
		exit(-1);
	}
	else {
		buildInputFile(argv);
		loadRules();

		Data* rootData = createData(head_left, head_right, NULL);
    	createNode(NULL, rootData);
   		
    	match(root);

    	ids();
    	pretty();
	}

	/*clock_t end = clock();
	double time = (double)(end - begin) / CLOCKS_PER_SEC;*/

    return 0;
    
}
