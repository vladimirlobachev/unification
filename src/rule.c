

#include "rule.h"

/* tag||initial form||final form||number of conditions:condition 1(type, cond, pred: if specified):condition2:...||mappings */
 
Rule *rules[NUMRULES];

int rule_counter = 0;

void addRule(Rule* rule) {
    rules[rule_counter] = rule;
    rule_counter++;
}

void ruleConstructor(char desc[]) {
    char *tokenDesc, *saveptrRule;
    const char delimBars[3] = "||";
    
    Rule* rule = malloc(sizeof(Rule));
    tokenDesc = strtok_r(desc, delimBars, &saveptrRule);
    rule->tag = malloc(strlen(tokenDesc));
    strcpy(rule->tag, tokenDesc);
    tokenDesc = strtok_r(NULL, delimBars, &saveptrRule);
    int count = 0;
    while (tokenDesc != NULL) {
        switch(count) {
            case 0:
                rule->probPart = malloc(strlen(tokenDesc));
                strcpy(rule->probPart, tokenDesc);
                break;
                
            case 1:
                rule->conclusion = getPositions(tokenDesc, rule->probPart, 1);
                break;
                
            case 2:
                conditionConstructor(rule, tokenDesc, rule->probPart);
                break;
                
            case 3:
                rule->mapping = getPositions(tokenDesc, rule->probPart, 0);
                break;
        }
        count++;
        tokenDesc = strtok_r(NULL, delimBars, &saveptrRule);
    }
    if (count == 2) { // if no conditions nor mappings
        rule->size = 0;
        rule->mapping = NULL;
    }
    else if (count == 3) {
        rule->mapping = NULL; // if no mappings only
    }
    addRule(rule);
}

void conditionConstructor(Rule* rule, char* desc, char* initialForm) {
    char *tokencond, *tt, *saveptrcond;
    
    tokencond = strtok_r(desc, ":", &saveptrcond);
    int num = tokencond[0]-'0';

    rule->condArray = malloc(sizeof(struct Condition*)*num);
    rule->size = num;
    int i=0;
    for (; i<num; i++) {
        rule->condArray[i] = malloc(sizeof(struct Condition));
        Condition* ptr = rule->condArray[i];
        tokencond = strtok_r(NULL, ":", &saveptrcond);
        ptr->type = tokencond[0];

        tt = strtok(tokencond, "(");
        tt = strtok(NULL, ")");
        switch(ptr->type) {
                
            case 't':
                createTCondition(ptr, getPositions(tt, initialForm, 0)); break;
                
            case 'm':
                createMCondition(ptr, tt[0]); break;
                
            case 'v':
                createVCondition(ptr, *getPositions(tt, initialForm, 0)); break;
                
            case 'i':
                createICondition(ptr, getPositions(tt, initialForm, 0)); break;        
            
        }
    }
}

char* getPositions(char *desc, char *initialForm, bool form) {
    char *output = malloc(sizeof(desc));
    memcpy(output, desc, strlen(desc)+1);
    int count = 0;
    int pos;
    int i=0;
    for (; i<strlen(initialForm); i++) {
        if (isalpha(initialForm[i])) {
            if (initialForm[i] == 'e') {
                i += 4;
                continue;
            }
            count++;
            if (strchr(desc, initialForm[i]) && (!isalpha(desc[strchr(desc, initialForm[i])-desc+1]))) {
                pos = (int)(strchr(desc, initialForm[i])-desc);
                if (!form) {
                    if (isupper(initialForm[i])) {
                        strcat(output, "u");
                    }
                    output[pos] = count + '0';
                    if (strchr(desc, initialForm[i]) != strrchr(desc, initialForm[i])) {        // x->y:x' or y->x:y'
                        output[(int)(strrchr(desc, initialForm[i])-desc)] = count + '0';
                    }
                    if (pos+1 == strlen(desc)) {
                        break;
                    }
                }
                else {
                    if (isalpha(desc[pos-1])) {
                        pos = (int)(strrchr(desc, initialForm[i])-desc);
                    }
                    output[pos] = count + '0';
                    if (count%2) { // indicates wrong positions
                        if (isupper(initialForm[i])) {
                            strcat(output, "u");
                        }
                    }
                    else {
                        if (islower(initialForm[i])) {
                            strcat(output, "l");
                        }
                    }
                }    
            }
        }
        if (initialForm[i] == '=') {
            count = 2;
        }
    }

    return output;
}

void createTCondition(Condition* cond, char *desc) {
    int pos;
    cond->leftPred = desc[0] - '0';
    switch(desc[1]) {
            
        case '<':
            cond->condition = -1;
            if (desc[2] == '=') {
                cond->extra = 0;
            }
            else {
                cond->extra = -1;
            } break;
            
        case '>':
            cond->condition = 1;
            if (desc[2] == '=') {
                cond->extra = 0;
            }
            else {
                cond->extra = -1;
            } break;
            
        case '=':
            cond->condition = 0;
            cond->extra = -1;
            break;
            
        case '!':
            cond->condition = 2;
            cond->extra = -1;
            break;
            
    }
    if (!cond->extra) {
        pos = 3;
    }
    else {
        pos = 2;
    }
    if (isupper(desc[pos])) {
        if (desc[pos] == 'L') {
            cond->tp = 4;
        }
        else {
            cond->tp = 3;
        }
        cond->rightPred = 0;
    }
    else {
        cond->rightPred = desc[pos] - '0';
        cond->tp = 0;
    }
    if (desc[strlen(desc)-1] == 'u') {
        cond->simple = 1;
    }
    else {
        cond->simple = 0;
    }
}

void createMCondition(Condition* cond, char desc) {
    switch(desc) {
        case '=': cond->condition = 1; break;
        case '!': cond->condition = 0; break;
    }
}

void createVCondition(Condition* cond, char desc) {
    cond->condition = desc - '0';
}

void createICondition(Condition* cond, char *desc) {
    if (desc[strlen(desc)-1] == 'u') {
        cond->condition = 1;
    }
    else {
        cond->condition = 0;
    }
}


