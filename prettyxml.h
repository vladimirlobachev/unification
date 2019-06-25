#ifndef prettyxml_h
#define prettyxml_h

#include "tree.h"
#include "concat.h"

#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 4096

void createHeader();
void buildXMLFile();
void goThrough(NTree* node);
char* buildNode(NTree* node);
void pushx(char* output, int degree);
char* getMapps(char* mappings[]);


#endif