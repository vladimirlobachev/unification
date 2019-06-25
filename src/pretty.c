
#include "pretty.h"

extern NTree* root;

char depth[4096];
int di;


void pretty() {
    printf("\n--------- Result Tree in Depth-First Order --------\n\n");
    printNodes(root);
    printf("------------------ Tree structure -----------------\n\n");
    prettyTree(root);
    printf("\n");
}


void push(char c) {
	depth[di++] = c;
	depth[di++] = ' ';
	depth[di++] = ' ';
	depth[di++] = ' ';

}


void pull() {
	di -= 4;
	depth[di] = '\0';
}


void prettyTree(NTree* node) {
	printf("%d\n", node->id);

	NTree* child = node->child;

	while(child) {
		NTree* bro = child->brother;
		printf( "%s `--", depth );
		if (bro) {
			push('|');
		}
		else {
			push(' ');
		}
		prettyTree(child);
		pull();
		child = bro;
	}

}