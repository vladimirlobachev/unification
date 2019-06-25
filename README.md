# unification
implementation of rule-based unification algorithm for GP 2 list expressions

I implemented rule-based unification algorithm in C for GP 2 list expressions. GP 2 is a very abstract, rule-based programming language for solving graph programs which is based on Graph Theory, where list expressions are graph labels (say, I/O). The aim was to implement an automatic program analysis for detecting confluence in graph programs (a graph program is confluent if all its executions on a given input will produce the same result). The algorithm (explained in few words) takes as an input a list expression and matches it with a set of inference rules. If at least one of the rules is matched, the input is transformed according to the conclusion part of the rule and the process repeats.
