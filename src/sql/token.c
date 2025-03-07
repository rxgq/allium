#include <stdlib.h>
#include "token.h"

Token *init_token(char *lexeme, TokenType type) {
  Token *token = (Token *)malloc(sizeof(Token));
  token->lexeme = lexeme;
  token->type = type;

  return token;
}