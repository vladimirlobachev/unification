

#ifndef transf_h
#define transf_h

#include "list.h"
#include "entry.h"
#include "rule.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>



int transform(Rule* rule, NTree* node);
void concatFromSimp(bool side, Expression* var);
void getMappings(char* mapping, NTree* node, char* str);
bool checkForRemoval(char* mappings[], char* replacement, char* var);
void getMappingsSimp(Expression* sid, char* str);
void mappcpy(Data* prev, Data* current);


#endif /* transf_h */
