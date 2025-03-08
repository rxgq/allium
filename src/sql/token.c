#include <stdlib.h>
#include <string.h>

#include "token.h"

Token *init_token(char *lexeme, TokenType type) {
  Token *token = (Token *)malloc(sizeof(Token));
  token->lexeme = strdup(lexeme);
  token->type = type;

  return token;
}

char *token_type_to_str(TokenType type) {
  switch (type) {
    case TOKEN_SELECT: return "select";
    case TOKEN_FROM: return "from";
    case TOKEN_IDENTIFIER: return "identifier";
    case TOKEN_BAD: return "bad token";
    case TOKEN_STAR: return "star";
    case TOKEN_NONE: return "none";
  }

  return "unknown type";
}