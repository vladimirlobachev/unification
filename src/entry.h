
#ifndef entry_h
#define entry_h
#define NUMTYPES 5 /* number of all possible types */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef enum {Char, String, Integer, Atom, List} Type;

typedef struct Entry {
    bool var; // 1 for var; 0 for not var
    Type type;
    char *term;
} Entry;

struct Entry* create_entry(Type type, char *entry, bool var);
bool isVar(Entry* expr);
Type type(Entry* expr);
int compare(Type t1, Type t2);


#endif /* entry_h */
