#include <stdlib.h>
#include <string.h>

#include "token.h"

char *token_type_to_str(TokenType type) {
  switch (type) {
    case TOKEN_SELECT: return "select";
    case TOKEN_FROM: return "from";
    case TOKEN_IDENTIFIER: return "identifier";
    case TOKEN_BAD: return "bad token";
    case TOKEN_STAR: return "star";
    case TOKEN_NONE: return "none";
    case TOKEN_AS: return "as";
    case TOKEN_COMMA: return "comma";
    case TOKEN_CREATE: return "create";
    case TOKEN_LEFT_PAREN: return "left parenthesis";
    case TOKEN_RIGHT_PAREN: return "right parenthesis";
    case TOKEN_TABLE: return "table";
    case TOKEN_DROP: return "drop";
    case TOKEN_EOF: return "eof";
  }

  return "unknown type";
}