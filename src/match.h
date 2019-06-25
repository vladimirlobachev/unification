
#ifndef match_h
#define match_h

#include "entry.h"
#include "list.h"
#include "rule.h"
#include "transf.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


void match(NTree* node);
void matchRules(NTree* node);
bool matchRule(Rule* rule, Expression* left, Expression* right);
bool matchForm(char* initialForm, Expression* left, Expression* right);
bool matchVariables(Entry* var1, Entry* var2);
bool matchSimpleExpressions(Expression* left, Expression* right);
bool matchFormOnSide(bool side, char* form, Expression* expr);
bool matchCond(Condition** condArray, int size, Expression* left, Expression* right);
bool match_t(Condition* cond, Expression* left, Expression* right);
bool match_v(int condition, Expression* left, Expression* right);
bool match_m(Expression* left, Expression* right, int condition);
bool matchCondSimp(Expression* left, Expression* right);


#endif /* match_h */
