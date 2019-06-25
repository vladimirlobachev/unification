CC = gcc
CFLAG = -c
PFLAG = -d

LEX = flex
PARS = bison

DEPS = parser.tab.h parser.tab.c lex.yy.c


OBJS = main.o entry.o list.o rule.o \
       match.o tree.o input.o transf.o \
       pretty.o


unify: $(OBJS)
	$(CC) -o $@ parser.tab.c lex.yy.c $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAG) $<

parser.tab.h parser.tab.c: parser.y
	$(PARS) $(PFLAG) parser.y

lex.yy.c: parser.tab.h lexer.l
	$(LEX) lexer.l

.PHONY: clean

clean:
	rm unify $(OBJS) $(DEPS)