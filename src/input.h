

#ifndef input_h
#define input_h

#include "rule.h"
#include "parser.tab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 256


void loadRules();
void buildInputFile(const char *argv[]);
void validateInput();

#endif /* input_h */
