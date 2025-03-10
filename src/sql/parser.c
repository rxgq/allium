#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "expr.h"

static ParserState *parser;

ParserState *init_parser(Token *tokens, int token_count) {
  ParserState *parser = malloc(sizeof(ParserState));
  parser->ast = init_sql_tree();
  parser->tokens = tokens;
  parser->token_count = token_count;
  parser->current = 0;

  return parser;
}

void free_parser(ParserState *parser) {
  // to be continued

  free(parser);
}

static inline Token *current() {
  return &parser->tokens[parser->current];
}

static inline void advance() {
  parser->current++;
}

static inline int match(TokenType type) {
  return current()->type == type ? 1 : 0;
}

static int expect(TokenType type) {
  if (match(type)) {
    advance();
    return 1;
  }

  return 0;
}

// sets the previous token as the error token
static void parser_error() {
  parser->current--;
  parser->error_token = current();
  advance();
}

static SqlExpr *bad_expr() {
  parser_error();
  return init_expr(BAD_EXPR);
}

static inline int is_bad(SqlExpr *expr) {
  if (expr->type == BAD_EXPR) return 1;
  return 0;
}

static void add_expr(SqlExpr *expr) {
  if (parser->ast->statement_capacity >= parser->ast->statement_count) {
    parser->ast->statement_capacity *= 2;
    parser->ast->statements = realloc(parser->ast->statements, sizeof(SqlExpr) * parser->ast->statement_capacity);
  }

  parser->ast->statements[parser->ast->statement_count] = *expr;
  parser->ast->statement_count++;
}

static SqlExpr *parse_primary() {
  Token *token = current();

  if (token->type == TOKEN_IDENTIFIER) {
    advance();

    SqlExpr *expr = init_expr(EXPR_IDENTIFIER);
    expr->as.identifier.value = strdup(token->lexeme);

    return expr;
  }
  else if (token->type == TOKEN_STAR) {
    advance();

    SqlExpr *expr = init_expr(EXPR_ALL_COLUMNS);
    expr->as.all_columns.value = token->lexeme;

    return expr;
  }

  return bad_expr();
}

static SqlExpr *parse_alias() {
  SqlExpr *expr = parse_primary();
  if (is_bad(expr)) return expr;

  if (match(TOKEN_AS)) {
    advance();

    SqlExpr *alias = init_expr(EXPR_ALIAS);
    alias->as.alias.expr = expr;
    alias->as.alias.identifier = parse_primary();
    if (is_bad(alias->as.alias.identifier)) return alias->as.alias.identifier;

    return alias;
  }

  return expr;
}

static SqlExpr *parse_expr() {
  return parse_alias();
}

static SqlExpr *parse_from_clause() {
  if (!expect(TOKEN_FROM)) {
    return bad_expr();
  }

  SqlExpr *from = init_expr(EXPR_FROM_CLAUSE);

  SqlExpr *expr = parse_expr();
  if (is_bad(expr)) return expr;
  from->as.from_clause.expr = expr;

  return from;
}

static SqlExpr *parse_select_clause() {
  SqlExpr *expr = init_expr(EXPR_SELECT_CLAUSE);
  expr->as.select_clause.options_count = 0;
  expr->as.select_clause.options = malloc(sizeof(SqlExpr));

  parser->current--;

  int capacity = 1;
  do {
    advance();

    SqlExpr *identifier = parse_expr();
  if (is_bad(identifier)) return identifier;

    if (expr->as.select_clause.options_count >= capacity) {
      capacity *= 2;
      expr->as.select_clause.options = realloc(expr->as.select_clause.options, sizeof(SqlExpr) * capacity);
    }

    expr->as.select_clause.options[expr->as.select_clause.options_count] = *identifier;
    expr->as.select_clause.options_count++;

  } while (match(TOKEN_COMMA));

  return expr;
}

static SqlExpr *parse_select_stmt() {
  if (!expect(TOKEN_SELECT)) {
    return bad_expr();
  }

  SqlExpr *select_clause = parse_select_clause();
  if (is_bad(select_clause)) return select_clause;

  SqlExpr *from_clause = parse_from_clause();
  if (is_bad(from_clause)) return from_clause;

  SqlExpr *select = init_expr(EXPR_SELECT_STMT);

  select->as.select.clauses = malloc(sizeof(SqlExpr) * 2);
  select->as.select.clauses[0] = *select_clause;
  select->as.select.clauses[1] = *from_clause;

  return select;
}

static SqlExpr *parse_create_table_stmt() {
  if (!expect(TOKEN_CREATE)) {
    return bad_expr();
  }

  if (!expect(TOKEN_TABLE)) {
    return bad_expr();
  }

  SqlExpr *expr = init_expr(EXPR_CREATE_TABLE_STMT);

  SqlExpr *table_name = parse_expr();
  if (is_bad(table_name)) return table_name;

  expr->as.create_table.name = table_name;
  expr->as.create_table.column_count = 0;
  expr->as.create_table.columns = malloc(sizeof(ColumnDefinition));

  if (!expect(TOKEN_LEFT_PAREN)) {
    return bad_expr();
  }

  int capacity = 1;
  // int count = 1;
  parser->current--;
  do {
    advance();

    SqlExpr *type = parse_expr();
    if (is_bad(type)) return type;

    SqlExpr *column_name = parse_expr();
    if (is_bad(column_name)) return column_name;

    ColumnDefinition *column = malloc(sizeof(ColumnDefinition));
    column->name = column_name->as.identifier.value;
    column->type = type->as.identifier.value;

    expr->as.create_table.columns[expr->as.create_table.column_count] = *column;
    expr->as.create_table.column_count++;

  } while (match(TOKEN_COMMA));

  if (!expect(TOKEN_RIGHT_PAREN)) {
    return bad_expr();
  }

  return expr;
}

static SqlExpr *parse_stmt() {
  if (match(TOKEN_SELECT)) {
    return parse_select_stmt();
  }
  else if (match(TOKEN_CREATE)) {
    return parse_create_table_stmt();
  }

  return bad_expr();
}

static short get_num_len(int n) {
  if (n < 0) return get_num_len((n == INT_MIN) ? INT_MAX : -n);
  if (n < 10) return 1;
  return 1 + get_num_len(n / 10);
}

static void output_error() {
  int errTokLine = parser->error_token->line;
  int errTokCol = 0;

  printf("line %d: ", errTokLine);
  int prefixLen = get_num_len(errTokLine) + 5;

  for (int i = 0; i < parser->token_count; i++) {
    if (parser->tokens[i].line != errTokLine) continue;

    if (&parser->tokens[i] == parser->error_token) {
      errTokCol = prefixLen;
    }

    printf("%s ", parser->tokens[i].lexeme);
    prefixLen += strlen(parser->tokens[i].lexeme) + 1;
  }
  printf("\n");

  for (int i = 0; i < errTokCol; i++) {
    printf(" ");
  }
  printf("^\n");

  printf("  Syntax error on line %d, near '%s'\n", errTokLine, parser->error_token->lexeme);
}

ParserState *parse_ast(Token *tokens, int token_count) {
  parser = init_parser(tokens, token_count);

  while (!match(TOKEN_EOF)) {
    SqlExpr *expr = parse_stmt();
    add_expr(expr);

    if (is_bad(expr)) {
      output_error();
      break;
    }
  }

  // parser_out(parser);

  return parser;
}