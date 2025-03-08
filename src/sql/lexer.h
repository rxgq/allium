#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct {
  Token *tokens;
  unsigned int token_capacity;
  unsigned int token_count;
  unsigned int current;
  char *source;
} LexerState;

extern LexerState *tokenize(char *source);
extern void free_lexer(LexerState *lexer);

#endif