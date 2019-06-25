


#include "input.h"

extern FILE *yyin;
extern int yyparse();

FILE *inf;

char *inputfile = "input.txt"; 
char *rulesfile = "rules.txt";


void loadRules() {

    char* line = NULL;
    size_t len = 0;
    
    
    inf = fopen(rulesfile, "r");
    while (getline(&line, &len, inf) != -1) {
        line[strlen(line)-1] = '\0';
        ruleConstructor(line);
    }
    
    fclose(inf);
    
}

void buildInputFile(const char *argv[]) {
    bool none = true;
    inf = fopen(inputfile, "w");

    if (inf == NULL) {
        printf("Cannot open the file\n");
        exit(-1);
    }

    fprintf(inf, "%s", argv[1]);
    int i=4;
    for (;i>-1; i--) {
        if (!strstr(argv[6-i], "-")) {
            fprintf(inf, "\n%d %s", i, argv[6-i]);
            none = false;
        }
    }
    if (none) {
        fprintf(inf, "\nempty");
    }
    fprintf(inf, "\nend");
    fclose(inf);
    validateInput();

}


void validateInput() {
    yyin = fopen(inputfile, "r");
    yyparse();
    fclose(yyin);
}

