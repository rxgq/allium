#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct {
  Token *tokens;
  int token_capacity;
  int token_count;
  int current;
  char *source;
  int line;
} LexerState;

extern LexerState *tokenize(int debug, char *source);
extern void free_lexer(LexerState *lexer);

#endif