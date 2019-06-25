

#include "transf.h"


extern Expression* head_left;
extern Expression* head_right;


int transform(Rule* rule, NTree* node) {
    bool side = true;
    char *newTerm, *token, *form = malloc(strlen(rule->conclusion));
    
    strcpy(form, rule->conclusion);
    
    if (strstr(form, "fail")) {
        Data* newData = createData(NULL, NULL, rule->tag);
        newData->mappings[0] = "fail";
        newData->counter++;
        createNode(node, newData);
        free(form);
        return 0;
    }
    else  if (!strcmp(form, "empty")) {
        Data* newData = createData(NULL, NULL, rule->tag);
        if (rule->mapping == NULL) {
            if (!node->data->counter) {
                newData->mappings[0] = "solved";
                newData->counter++;
            }
            else {
                if (node->data->counter) {
                    mappcpy(node->data, newData);
                }   
                else {
                    newData->mappings[newData->counter] = "solved";
                    newData->counter++;
                }
            }
            /*
             rule Remove is matched, no further derivation
             to exclude Decomp3/Remove repetition
             */
            createNode(node, newData);
            free(form);
            return 0;
        }
        else {
            if (node->data->counter) {
                mappcpy(node->data, newData);
            }
            char* temp = malloc(sizeof(char));
            getMappings(rule->mapping, node, temp);
            char* primevar = malloc(strlen(temp)-strlen(strchr(temp, '-')));
            strncpy(primevar, temp, strlen(temp)-strlen(strchr(temp, '-')));
            if (primevar[1] == 39) {
                if (checkForRemoval(newData->mappings, strchr(temp, '>')+1, primevar)) {
                memset(temp, '\0', strlen(temp));
                free(temp);
                }
            }
            else {
                newData->mappings[newData->counter] = temp;
                newData->counter++;
            }
            newData->mappings[newData->counter] = "solved";
            newData->counter++;
            free(primevar);   
        }
        createNode(node, newData);
        free(form);
        return 1;
    }
    
    token = strtok(form, "=");
    
    while (token != NULL) {
        
        int i=0;
        for (; i<strlen(token); i++) {
            if (!isalpha(token[i])) {
                if (token[i] == ':') {
                    continue;
                }
                if (token[i+1] == 39) {
                    switch(token[i]-'0') {
                        case 1:
                            newTerm = malloc(strlen(getLeft(node)->entry->term)+1);
                            strcpy(newTerm, getLeft(node)->entry->term);
                            strcat(newTerm, "'");
                            concat(side, create_entry(List, newTerm, 1));
                            break;
                        case 3:
                            newTerm = malloc(strlen(getRight(node)->entry->term)+1);
                            strcpy(newTerm, getRight(node)->entry->term);
                            strcat(newTerm, "'");
                            concat(side, create_entry(List, newTerm, 1));
                            break;
                    }
                    continue;
                }
                switch(token[i]-'0') {
                    case 1: concat(side, node->data->left->entry); break;
                    case 2:
                        if (node->data->left->next != NULL) {
                            concatFromSimp(side, getLeft(node)->next);
                        }
                        break;
                    case 3:
                        if (strchr(rule->conclusion, 'u')) {
                            if (node->data->right) {
                                concatFromSimp(side, getRight(node));
                            }
                        }
                        else {
                            concat(side, getRight(node)->entry);
                        } 
                        break;
                    case 4:
                        if (strchr(rule->conclusion, 'l')) {
                            concat(side, getRight(node)->next->entry);
                        }
                        else {
                            
                            if (node->data->right->next != NULL) {
                                concatFromSimp(side, getRight(node)->next);
                            }
                            break;
                        }
                }
            }
            else {
                break; // break the iteration if empty Expression is found or 'l' or 'u'
            }
        }
        
        
        side = !side;
        token = strtok(NULL, "=");
    }

    Data* newData = createData(head_left, head_right, rule->tag);
    if (rule->mapping != NULL){
        if (node->data->counter) {
            mappcpy(node->data, newData);
        }
        char* temp = malloc(sizeof(char));
        getMappings(rule->mapping, node, temp);
        char* primevar = malloc(strlen(temp)-strlen(strchr(temp, '-')));
        strncpy(primevar, temp, strlen(temp)-strlen(strchr(temp, '-')));
        if (primevar[1] == 39) {
            if (checkForRemoval(newData->mappings, strchr(temp, '>')+1, primevar)) {
                memset(temp, '\0', strlen(temp));
                free(temp);   
            }
        }
        else {
        newData->mappings[newData->counter] = temp;
        newData->counter++;
        }
        free(primevar);
    }
    else {
        if (node->data->counter) {
            mappcpy(node->data, newData);
        }
        /* Decomp3 case */
        if (newData->left == NULL && newData->right == NULL) {
            if (!newData->counter) {
                newData->mappings[0] = "solved";
                newData->counter++;
            }
            else {
                if (node->data->counter) {
                    mappcpy(node->data, newData);
                    newData->mappings[newData->counter] = "solved";
                    newData->counter++;
                }
                else {
                    newData->mappings[newData->counter] = "solved";
                    newData->counter++;
                }
                
                
            }
        }
        
        
        
    }
    createNode(node, newData);
    free(form);
    return 1;
}

void concatFromSimp(bool side, Expression* var) {
    Expression* ptr = var;
    
    while (ptr != NULL) {
        concat(side, ptr->entry);
        ptr = ptr->next;
    }
    
}

void getMappings(char* mapping, NTree* node, char* str) {
    int i=0;
    for (; i<strlen(mapping); i++) {
        if (mapping[i] == '-') {
            i += 1;
            strcat(str, "->");
            continue;
        }
        if (!isalpha(mapping[i])) {
            switch(mapping[i]-'0') {
                case 1:
                    strcat(str, getLeft(node)->entry->term);
                    if (mapping[i+1] == 39) {
                        strcat(str, "'");
                        i += 1;
                        break;
                    }
                    if (mapping[i+1] == ':') {
                        strcat(str, ":");
                    }
                    break;
                case 3:
                    if (!strchr(mapping, 'u')) {
                        strcat(str, getRight(node)->entry->term);
                        if (mapping[i+1] == 39) {
                            strcat(str, "'");
                            i += 1;
                            break;
                        }
                        if (mapping[i+1] == ':') {
                            strcat(str, ":");
                        }
                        break;
                    }
                    if (node->data->right != NULL) {
                        getMappingsSimp(getRight(node), str);
                        break;
                    }
                    else {
                        strcat(str, "empty");
                    }
            }
        }
        else {
            if (mapping[i] != 'u') {
                strcat(str, "empty");
            }
            break;
        }
    }
}

bool checkForRemoval(char* mappings[], char* replacement, char* var) {
    int i=0;
    for (; i<MAXSIZE/2; i++) {
        if (mappings[i] == NULL) {
            return 0;
        }
        if (strstr(strchr(mappings[i], '>')+1, var)) {
            char* temp = strstr(mappings[i], var);
            char* new = malloc(sizeof(mappings[i])-strlen(temp));
            if (!strcmp(replacement, "empty")) {
                strncpy(new, mappings[i], strlen(mappings[i]) - strlen(temp) - 1);
            }
            else {
                strncpy(new, mappings[i], strlen(mappings[i]) - strlen(temp));
                strcat(new, replacement);
            }
            mappings[i] = new;
            return 1;
        }
    }
    return 0;
    
}

void getMappingsSimp(Expression* sid, char* str) {
    Expression* ptr = sid;
    while (ptr != NULL) {
        strcat(str, ptr->entry->term);
        if (ptr->next != NULL) {
            strcat(str, ":");
        }
        else {
            
            break;
        }
        ptr = ptr->next;
    }
}


void mappcpy(Data* prev, Data* current) {
    int i=0;
    for (; i<prev->counter; i++) {
        current->mappings[i] = prev->mappings[i];
    }
    current->counter = prev->counter;
    return;
}


