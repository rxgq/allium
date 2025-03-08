CC = gcc
EXEC = main

SRC = src/main.c
SQL = src/sql/lexer.c src/sql/token.c src/sql/parser.c src/sql/expr.c
STORE = src/store/page.c src/store/store.c

all: 
	$(CC) $(SRC) $(SQL) $(STORE) -o $(EXEC)