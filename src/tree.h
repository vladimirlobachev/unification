

#ifndef tree_h
#define tree_h

#include "list.h"
#include "entry.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>



typedef struct Data {
    struct Expression* left;
    struct Expression* right;
    char *ruletag;
    char *mappings[MAXSIZE/2];
    int counter;
} Data;

typedef struct NTree {
	int id;
    struct Data* data;
    struct NTree* brother;
    struct NTree* child;
} NTree;


Data* createData(Expression* left, Expression* right, char* ruletag);
void createRoot(Data* data);
void createNode(NTree* parent, Data* data);
Expression* getLeft(NTree* node);
Expression* getRight(NTree* node);
void ids();
void setIds(NTree* nodes[], int counter);
void printNodes(NTree* node);


#endif /* tree_h */
