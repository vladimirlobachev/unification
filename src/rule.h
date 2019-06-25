

#ifndef rule_h
#define rule_h

#include "entry.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define NUMRULES 20

typedef struct Rule {
    char* tag;
    char* probPart; // y:L=x:M (orient3)
    char* conclusion; // 1:2=3:4 (orient3)
    char* mapping; // x->a (decomp4)
    struct Condition** condArray;
    int size;
} Rule;

typedef struct Condition {
    char type;          // 't': type(x) = type(y); 'e': L != empty; 'v': a is not variable; 'm': x in or not in Var(L); 'i'; x != L
    int condition;
    int extra;
    int leftPred;
    int rightPred;
    int tp;
    bool simple;
} Condition;

void addRule(Rule* rule);
void ruleConstructor(char desc[]);
void conditionConstructor(Rule* rule, char *desc, char *initialForm);
char* getPositions(char *desc, char *initialForm, bool form);
void createTCondition(Condition* cond, char *desc);
void createMCondition(Condition* cond, char desc);
void createVCondition(Condition* cond, char desc);
void createICondition(Condition* cond, char *desc);



#endif /* rule_h */
