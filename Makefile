CC = gcc
EXEC = main

SRC = src/main.c
OBJS= src/lexer.c src/token.c src/expr.c src/parser.c src/table.c src/db.c

all: 
	$(CC) $(INCLUDES) $(SRC) $(OBJS) -o $(EXEC)