

#include "match.h"


extern Expression* head_left;
extern Expression* head_right;

extern Rule* rules[];


void match(NTree* node) {
    if (node->data->counter)  {
        if (!strcmp(node->data->mappings[0], "fail")) {
            return;
        }
    }
    matchRules(node);
    if (node->brother) {
        match(node->brother);
    }
    if (node->child) {
        match(node->child);
    }
}

void matchRules(NTree* node) {
    bool matched = false;
    Rule* rule = rules[0];
    int rule_counter = 1;
    while (rule_counter < NUMRULES+1) {
        if (!strcmp(rule->conclusion, "fail") && matched) {
            break;
        }
        if (getLeft(node) || getRight(node)) {
            if (matchRule(rule, getLeft(node), getRight(node))) {
                if (strcmp(rule->conclusion, "fail") && !matched) {
                    matched = !matched;
                }
                if (!transform(rule, node)) {
                    return;
                }
            }
            rule = rules[rule_counter];
            rule_counter++;
        }
        else {
            break;
        }
    }
}

bool matchRule(Rule* rule, Expression* left, Expression* right) {
    if (matchForm(rule->probPart, left, right)) {
        return matchCond(rule->condArray, rule->size, left, right);
    }
    return 0;
}

bool matchForm(char* initialForm, Expression* left, Expression* right) {
    char *sides, *saveptrSides;
    bool side = true;
    if (strchr(initialForm, initialForm[0]) == strrchr(initialForm, initialForm[0])) {
        if (left && right) {
            if (!isupper((strchr(initialForm, '=')+1)[0])) { 
                /* if there is an simp on the right side then the first var on the left is compared with the one on the right 
                   to eliminate unneccasary computation.
                */
                if (matchVariables(left->entry, right->entry)) { 
                    return 0;
                }   
            }
        }
        sides = strtok_r(strdup(initialForm), "=", &saveptrSides);
        while (sides != NULL) {
            if (side) {
                if (!matchFormOnSide(side, sides, left)) {
                    return 0;
                }
            }
            else {
                return (matchFormOnSide(side, sides, right));
            }
            side = !side;
            sides = strtok_r(NULL, "=", &saveptrSides);
        }

        return 0;
    }
    else {
        if (!isupper(initialForm[0])) {
            if (left && right) {
                return (matchVariables(left->entry, right->entry));
            }
            return 0;
        }
        else {
            return (matchSimpleExpressions(left, right));
        }
    }
}


bool matchFormOnSide(bool side, char* form, Expression* expr) {
    char *var;
    int count = -1;
    bool simple = 0;
    
    var = strtok(form, ":");
    Expression* toCheck = expr;
    while (var != NULL) {
        count++;
        if (toCheck == NULL) {
            if (!(var[0] == 'L' || var[0] == 'M' || !strcmp(var, "empty"))) {
                return 0;
            }
            break;
        }
        else {
            if (count) {
                if (toCheck->next != NULL) {
                    toCheck = toCheck->next;
                }
                else {
                    if (!(var[0] == 'L' || var[0] == 'M' || !strcmp(var, "empty"))) {
                        return 0;
                    }
                    break;
                }
            }
            if (var[0] == 'x' || var[0] == 'y' || var[0] == 'z') {
                if (!isVar(toCheck->entry)) {
                    return 0;
                }
            }
            else if (var[0] == 'a' || var[0] == 'b') {
                if (!isVar(toCheck->entry)) {
                    if (!(type(toCheck->entry) < Atom)) { // char/string/integer
                        return 0;
                    }
                }
                else {
                    if (!(type(toCheck->entry)!= List)) { // char/string/integer/atom
                        return 0;
                    }
                }
            }
            else if (var[0] == 's' || var[0] == 't') {
                if (!isVar(toCheck->entry)) {
                    if (!(type(toCheck->entry) < Atom)) { // char/string/Integer
                        return 0;
                    }
                }
            }
            else if (!strcmp(var, "empty")) {
                return 0;
            }
            else {
                simple = !simple;
            }
            
        }
        var = strtok(NULL, ":");
    }
    
    if (!count && (toCheck != NULL)) { // subst1, decomp4, clash4, occur
        if (toCheck->next != NULL && !simple) {
            return 0;
        }
    }
    
    return 1;
}


bool matchCond(Condition** condArray, int size, Expression* left, Expression* right) {
    for (int i=0; i<size; i++) {
        switch(condArray[i]->type) {
            case 't': if (!match_t(condArray[i], left, right)) { return 0; } break;
            case 'e': if (left->next == NULL) { return 0; } break;
            case 'v': if (!match_v(condArray[i]->condition, left, right)) { return 0; } break;
            case 'm': if (!match_m(left, right, condArray[i]->condition)) { return 0; } break;
            case 'i':
                if (!condArray[i]->condition) {
                    if (matchVariables(left->entry, right->entry)) { return 0; } break;
                }
                if (!matchCondSimp(left, right)) { return 0; } 
                break;
        }
    }
    return 1;
}

bool matchVariables(Entry* var1, Entry* var2) {
    if (var1 && var2) {
        if (isVar(var1) == isVar(var2)) {
            if (!compare(type(var1), type(var2))) {
                if (!strcmp(var1->term, var2->term)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

bool matchSimpleExpressions(Expression* left, Expression* right) {
    Expression* leftptr = left;
    Expression* rightptr = right;
    if (leftptr == NULL || rightptr == NULL) {
        return 0; 
    }
    
    while (leftptr != NULL) {
        if (rightptr == NULL) {
            return 0;
        }
        if (matchVariables(leftptr->entry, rightptr->entry)) {
            leftptr = leftptr->next;
            rightptr = rightptr->next;
            continue;
        }
        
        return 0;
    }
    
    if (rightptr == NULL) { return 1; } return 0;
}

bool match_t(Condition* prem, Expression* left, Expression* right) {
    switch(prem->leftPred) {
        case 1:
            if (prem->rightPred) {  
                if (prem->simple) {
                    Type tp = 0;
                    if (right) {   
                        Expression* tExpression = right;
                        while (tExpression) {
                            if (tp < type(tExpression->entry)) {
                                tp = type(tExpression->entry);
                            }
                            tExpression = tExpression->next;
                        }
                    }
                    else {
                        tp = List;
                    }

                    if (!prem->extra) {
                        if (!(compare(type(left->entry), tp)==prem->extra) &&
                            !(compare(type(left->entry), tp)==prem->condition)) {
                            return 0;
                        }
                        break;
                    }
                    if (!(compare(type(left->entry), tp)==prem->condition)) {
                        return 0;
                    }
                    break;
                }
                if (!prem->extra) {
                    if (!(compare(type(left->entry), type(right->entry))==prem->extra) &&
                        !(compare(type(left->entry), type(right->entry))==prem->condition)) {
                        return 0;
                    } break;
                }
                if (!(compare(type(left->entry), type(right->entry))==prem->condition)) {
                    return 0;
                } break;
            }
            if (!prem->extra) {
                if (!(compare(type(left->entry), prem->tp)==prem->extra) &&
                    !(compare(type(left->entry), prem->tp)==prem->condition)) {
                    return 0;
                } break;
            }
            if (!(compare(type(left->entry), prem->tp)==prem->condition)) {
                return 0;
            } break;
        case 3: if (!(compare(type(right->entry), prem->tp)==prem->condition)) { return 0; } break;
        case 4: if (!(compare(type(right->next->entry), prem->tp)==prem->condition)) { return 0; } break;
    }
    return 1;
}

bool match_v(int condition, Expression* left, Expression* right) {
    switch(condition) {
        case 1: if (!isVar(left->entry)) { return 1; } break;
        case 3: if (!isVar(right->entry)) { return 1; } break;
    }
    return 0;
}

bool match_m(Expression* left, Expression* right, int condition){
    if (right == NULL) { // if there is no simple expressions on the right side, then var definetely does not belong to it
        if (condition) { return 0; }
        else { return 1; }
    }
    Expression* pointer = right;
    while (pointer != NULL) {
        if (matchVariables(left->entry, pointer->entry)) {
            if (condition) { return 1; }
            else { return 0; }
        }
        
        pointer = pointer->next;
    }
    if (!condition) { return 1; }
    return 0;
}

bool matchCondSimp(Expression* left, Expression* right) {
    if (right) {
        if (right->next == NULL) {
            return (!matchVariables(left->entry, right->entry));
        }
    }
    return 1;
}
