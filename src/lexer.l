%option noyywrap

%x types


%{

#include "parser.tab.h"

int line = 0;

%}
%%

\: 										{ return ':'; }
\= 										{ return '='; }
\n 										{ line++; BEGIN types; }

[a-z]                                   { yylval.var = yytext[0]; return VAR; }
\"(\\.|[^\\"])*\" 						{ yylval.nvar = strdup(yytext); return NVAR; }
[1-9][0-9]*								{ yylval.nvar = strdup(yytext); return NVAR; }
empty									{ return EMPTY; }
.										{ printf("Unrecognised characters: "); ECHO; printf("\n"); exit(-1); }


<types>empty							{ return EMPTY; }
<types>[0-4][ ]							{ yylval.type = atoi(&yytext[0]); return TYPE; }
<types>[a-z][\,]						{ yylval.var = yytext[0]; return VAR; }
<types>[a-z][\n]						{ line++; yylval.var = yytext[0]; return VAR; }
<types>end 								{ return END; }
<types>.								{ printf("Unrecognised characters: "); ECHO; printf("\n"); exit(-1); }


%%
