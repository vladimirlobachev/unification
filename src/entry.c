

#include "entry.h"



struct Entry* create_entry(Type type, char *entry, bool var) {
    struct Entry* new_entry = malloc(sizeof(struct Entry));
    new_entry->type = type;
    new_entry->var = var;
    if (var) {
        if (entry[1] == 39) {
            new_entry->term = entry;
        }   
        else {
            char* label = malloc(sizeof(char));
            strncpy(label, entry, 1);
            new_entry->term = label;
        }   
    }
    else {
        new_entry->term = entry;
    }
    
    return new_entry;
}

int comparisons[NUMTYPES][NUMTYPES] = {             //          Char String Integer Atom List
        {0,-1,2,-1,-1},                             //     Char[  0    -1      2     -1   -1 ]
        {1, 0,2,-1,-1},                             //   String[  1     0      2     -1   -1 ]
        {2, 2,0,-1,-1},                             //  Integer[  2     2      0     -1   -1 ]
        {1, 1,1, 0,-1},                             //     Atom[  1     1      1      0   -1 ]
        {1, 1,1, 1, 0}                              //     List[  1     1      1      1    0 ]
    };

int compare(Type t1, Type t2) {              /*        0: t1 = t2    1: t1 > t2    -1: t1 < t2    2: t1||t2         */
    return comparisons[t1][t2];
}

bool isVar(Entry* expr) {
    return expr->var;
}

Type type(Entry* expr) {
    return expr->type;
}

