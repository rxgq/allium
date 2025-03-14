CC = gcc
EXEC = main
CFLAGS = -Wall -Wextra

SRC = src/main.c
OBJS= src/lexer.c src/token.c src/expr.c src/parser.c src/table.c src/db.c

all: 
	$(CC) $(CFLAGS) $(SRC) $(OBJS) -o $(EXEC)