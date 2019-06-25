
#ifndef list_h
#define list_h

#include "entry.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define MAXSIZE 64



// linked-Expression structure Expression denotes the concatenation of variables on each side of expression
typedef struct Expression {
    struct Entry* entry;
    struct Expression* next;
} Expression;

void create_expression(bool side, Entry* entry);
void concat(bool side, Entry* entry);
void nullExpressions();
char* getExpressions(Expression* left, Expression* right);
void printExpressions(Expression* left, Expression* right);
void printExpression(Expression* ptr);



#endif /* list */
