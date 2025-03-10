#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "expr.h"

static ParserState *parser;

static void init_parser(Token *tokens) {
  parser = malloc(sizeof(ParserState));
  parser->ast = init_sql_tree();
  parser->tokens = tokens;
  parser->current = 0;
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

static SqlExpr *bad_expr() {
  return init_expr(BAD_EXPR);
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
  advance();

  if (token->type == TOKEN_IDENTIFIER) {
    SqlExpr *expr = init_expr(EXPR_IDENTIFIER);
    expr->as.identifier.value = strdup(token->lexeme);

    return expr;
  }
  else if (token->type == TOKEN_STAR) {
    SqlExpr *expr = init_expr(EXPR_ALL_COLUMNS);
    expr->as.all_columns.value = token->lexeme;

    return expr;
  }

  printf("\nAn unknown expression was found while parsing expression: %d\n", token->lexeme);
  return bad_expr();
}

static SqlExpr *parse_alias() {
  SqlExpr *expr = parse_primary();

  if (match(TOKEN_AS)) {
    advance();

    SqlExpr *alias = init_expr(EXPR_ALIAS);
    alias->as.alias.expr = expr;
    alias->as.alias.identifier = parse_primary();

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
  SqlExpr *from_clause = parse_from_clause();

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
  expr->as.create_table.name = table_name;
  expr->as.create_table.column_count = 0;
  expr->as.create_table.columns = malloc(sizeof(ColumnDefinition));

  if (!expect(TOKEN_LPAREN)) {
    return bad_expr();
  }

  int capacity = 1;
  // int count = 1;
  parser->current--;
  do {
    advance();

    SqlExpr *type = parse_expr();
    SqlExpr *column_name = parse_expr();

    ColumnDefinition *column = malloc(sizeof(ColumnDefinition));
    column->name = column_name->as.identifier.value;
    column->type = type->as.identifier.value;

    expr->as.create_table.columns[expr->as.create_table.column_count] = *column;
    expr->as.create_table.column_count++;

  } while (match(TOKEN_COMMA));

  if (!expect(TOKEN_RPAREN)) {
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

SqlQueryTree *parse_ast(Token *tokens) {
  init_parser(tokens);

  while (!match(TOKEN_EOF)) {
    SqlExpr *expr = parse_stmt();
    add_expr(expr);

    if (expr->type == BAD_EXPR) {
      break;
    }
  }

  parser_out(parser);

  return parser->ast;
}