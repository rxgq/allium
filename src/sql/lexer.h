#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct LexerState {
  Token *tokens;
  unsigned int token_capacity;
  unsigned int token_count;
  unsigned int current;
  char *source;
} LexerState;

extern LexerState *init_lexer(char *source);
extern void tokenize(LexerState *lexer);

#endif