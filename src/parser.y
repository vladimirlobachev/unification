%{

#include "list.h"
#include "entry.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
    

    char vbuff[MAXSIZE+1];
    char *nvbuff[MAXSIZE/2];
    int tbuff[MAXSIZE+1];

    int vcounter = 0;
    int ncounter = 0;
    int tcounter = 0;

    int lcounter = 0;
    int list_pos;
    
    int yylex();
    extern int line;

    void yyerror(const char *s);
    void error_reporting(const char *error, int pos);
    void error_out(const char* error, char* var);

    void list();

%}

%union {
    char var;
    char *nvar;    
    int type;
    int pos[256];
}

%type <pos> variables 

%token <var> VAR
%token <nvar> NVAR
%token <type> TYPE

%token EMPTY END


%%

init:
    expression '=' expression types END                 { list(); }
    ;
expression: 
    entry                                               { if (!strchr(vbuff, '=')) 
                                                            {
                                                            if (vcounter > MAXSIZE/4*3) 
                                                                {
                                                                char *num = malloc(sizeof(char)*3);
                                                                sprintf(num, "%d", MAXSIZE/4*3);
                                                                error_out("The number of variables in the expression exceeds the limit:",
                                                                num);
                                                                }
                                                            vbuff[(int)(strchr(vbuff, '\0')-vbuff)] = '=';
                                                            vcounter++; 
                                                            } 
                                                        }
    | entry ':' expression 
    ; 
entry: 
    VAR                                                 { if (vcounter > MAXSIZE/4*3) 
                                                            {
                                                            char *num = malloc(sizeof(char)*3);
                                                            sprintf(num, "%d", MAXSIZE/4*3);
                                                            error_out("The number of variables in the expression exceeds the limit:",
                                                            num);
                                                            }
                                                          vbuff[(int)(strchr(vbuff, '\0')-vbuff)] = $1; vcounter++; }
                                                        
                                                         
                                                        
                                                      
                                                        
    | NVAR                                              { if (ncounter+1 > MAXSIZE/4)   
                                                            {
                                                            char *num = malloc(sizeof(char)*3);
                                                            sprintf(num, "%d", MAXSIZE/4);
                                                            error_out("The number of non-variables in the expression exceeds the limit:",
                                                            num);
                                                            }
                                                          nvbuff[ncounter] = $1; 
                                                          vbuff[(int)(strchr(vbuff, '\0')-vbuff)] = ncounter + '0';
                                                          ncounter++; 
                                                        }
    | EMPTY 
    ;
types:                                                  
    types typeline
    | typeline
    | EMPTY                                             
    ;
typeline:
    TYPE variables                                      { for (int i=0; i<tcounter; i++) 
                                                            {
                                                            if ($1 == 4) 
                                                                {
                                                                if (lcounter == 2) 
                                                                    {
                                                                    error_reporting("There must be at most one list variable on each side of the expression:", $2[i]);
                                                                    }
                                                                else if (lcounter == 1) 
                                                                    {
                                                                    int sec_list_pos = (int)(strchr(vbuff, vbuff[$2[i]])-vbuff);
                                                                    if (vbuff[list_pos] == vbuff[sec_list_pos]) 
                                                                        {
                                                                        error_reporting("Expression cannot have repeated list variables:", sec_list_pos);
                                                                        }
                                                                    else 
                                                                        {
                                                                        int mid = (int)(strchr(vbuff, '=')-vbuff);
                                                                        if ((list_pos < mid && sec_list_pos < mid) ||
                                                                            (list_pos > mid && sec_list_pos > mid)) 
                                                                                {
                                                                                error_reporting("There must be at most one list variable on each side of the expression:", sec_list_pos);
                                                                                }
                                                                        }
                                                                    }

                                                                lcounter++;
                                                                list_pos = (int)(strchr(vbuff, vbuff[$2[i]])-vbuff);
                                                                }
                                                            if (!tbuff[$2[i]]) 
                                                                {
                                                                tbuff[$2[i]] = $1 + 1;
                                                                }
                                                            else
                                                                {
                                                                error_reporting("Variable cannot have multiple types:", $2[i]);
                                                                }
                                                            } 
                                                            tcounter = 0;
                                                        }
    ;
variables:
    variables VAR                                       { if (!strchr(vbuff, $2)) 
                                                                {
                                                                error_out("Expressions do not include variable:", &($2));
                                                                }
                                                          char *pch = strchr(vbuff, $2);
                                                          while (pch) 
                                                                {
                                                                $$[tcounter] = (int)(pch-vbuff);
                                                                tcounter++;
                                                                pch = strchr(pch+1, $2);
                                                                }
                                                                                       
                                                        }
    | VAR                                               { if (!strchr(vbuff, $1)) 
                                                                {
                                                                error_out("Expressions do not include varibale", &($1));
                                                                }
                                                          char *pch = strchr(vbuff, $1);
                                                          while (pch) 
                                                                {
                                                                $$[tcounter] = (int)(pch-vbuff);
                                                                tcounter++;
                                                                pch = strchr(pch+1, $1);
                                                                }
                                                        }
    ;

%%

void list() {
    
    if (vbuff[0] == '=' && !vbuff[1]) {
        printf("Error: Problem is empty\n");
        exit(-1);
    }

    bool side = true;
    char *var = malloc(sizeof(char));
    for (int i=0; i<MAXSIZE; i++) {
        if (isalpha(vbuff[i])) {
            if (tbuff[i]) {
                concat(side, create_entry(tbuff[i]-1, &vbuff[i], 1));
            }
            else {
                strncpy(var, &vbuff[i], 1);
                error_out("Variable need type specification:", var);
            }
        }
        else if (isdigit(vbuff[i])) {
            int t, pos = vbuff[i] - '0';
            if (*nvbuff[pos] == 34) {
                if (strlen(nvbuff[pos])<4) {
                    t = 0;
                }
                else {
                    t = 1;
                }
            }
            else {
                t = 2;
            }
            concat(side, create_entry(t, nvbuff[pos], 0));
        }
        else if (vbuff[i] == '=') {
            side = !side;
        }
        else {
            break;
        }
    }
    
}


void yyerror(const char *s) {
    printf("Error: %s \n", s);
    exit(-1);
}

void error_reporting(const char *error, int pos) {
    char* var = malloc(sizeof(char));
    strncpy(var, &vbuff[pos], 1);
    error_out(error, var);
}

void error_out(const char *error, char* var) {
    printf("Error: %s ", error);
    printf("%s\n", var);
    exit(-1);    
}
