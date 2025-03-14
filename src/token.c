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
    case TOKEN_TOP: return "top";
    case TOKEN_NUMERIC: return "numeric";
    case TOKEN_PERCENT: return "percent";
    case TOKEN_DISTINCT: return "distinct";
    case TOKEN_WHERE: return "where";
    case TOKEN_SEMI_COLON: return "semicolon";
    case TOKEN_SINGLE_EQUALS: return "single equals";
    case TOKEN_SINGLE_QUOTE: return "single quote";
    case TOKEN_AND: return "and";
    case TOKEN_OR: return "or";
    case TOKEN_NOT: return "not";
    case TOKEN_GREATER_THAN: return "greater than";
    case TOKEN_LESS_THAN: return "less than";
    case TOKEN_LESS_THAN_EQUALS: return "less than equals";
    case TOKEN_GREATER_THAN_EQUALS: return "greater than equals";
    case TOKEN_NOT_EQUAL: return "not equal";
    case TOKEN_IS: return "is";
    case TOKEN_NULL: return "null";
    case TOKEN_EOF: return "eof";
  }

  return "unknown type";
}