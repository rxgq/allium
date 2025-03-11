CC = gcc
EXEC = main

SRC = src/main.c
SQL = src/lexer.c src/token.c src/expr.c src/parser.c 
STORE = src/table.c src/db.c

all: 
	$(CC) $(INCLUDES) $(SRC) $(SQL) $(STORE) -o $(EXEC)