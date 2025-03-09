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

static int expect(TokenType type) {
  Token *token = current();

  if (token->type == type) {
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

static SqlExpr *parse_expr() {
  Token *token = current();
  advance();

  if (token->type == TOKEN_IDENTIFIER) {
    SqlExpr *expr = init_expr(EXPR_IDENTIFIER);
    expr->as.identifier.value = strdup(token->lexeme);

    return expr;
  }

  printf("\nAn unknown expression was found while parsing expression: %d\n", token->lexeme);
  return bad_expr();
}

static SqlExpr *parse_from_clause() {
  SqlExpr *from = init_expr(EXPR_FROM_CLAUSE);
  advance();

  SqlExpr *expr = parse_expr();
  from->as.from_clause.expr = expr;

  return from;
}

static SqlExpr *parse_select_stmt() {
  if (!expect(TOKEN_SELECT)) {
    return bad_expr();
  }

  SqlExpr *expr = parse_expr(); // identifier

  SqlExpr *select_clause = init_expr(EXPR_SELECT_CLAUSE);

  select_clause->as.select_clause.options = malloc(sizeof(SqlExpr));
  select_clause->as.select_clause.options[0] = *expr;

  SqlExpr *from_clause = parse_from_clause();

  SqlExpr *select = init_expr(EXPR_SELECT_STMT);

  select->as.select_stmt.clauses = malloc(sizeof(SqlExpr) * 2); // 2 for select and from
  select->as.select_stmt.clauses[0] = *select_clause;
  select->as.select_stmt.clauses[1] = *from_clause;

  return select;
}

static void print_expr(SqlExpr *expr, int depth) {
  switch (expr->type) {
    case EXPR_SELECT_STMT:
      printf("select ");
      for (int i = 0; i < 2; i++) {
        print_expr(&expr->as.select_stmt.clauses[i], depth + 1);
      }
      return;

    case EXPR_SELECT_CLAUSE:
      print_expr(&expr->as.select_clause.options[0], depth + 1);
      return;

    case EXPR_IDENTIFIER:
      printf("%s ", expr->as.identifier.value);
      return;

    case EXPR_FROM_CLAUSE:
      printf("from ");
      print_expr(expr->as.from_clause.expr, depth + 1);
      return;

    case BAD_EXPR:
      printf("bad expr");
      return;
      
    default: 
      printf("\n  unknown type '%d'", expr->type);
  }
}

static void parser_out() {
  // for (int i = 0; i < parser->ast->statement_count; i++) {
  //   printf("expr type: %d", parser->ast->statements[i].type);
  // }

  for (int i = 0; i < parser->ast->statement_count; i++) {
    print_expr(&parser->ast->statements[i], 1);
  }
}

SqlQueryTree *parse_ast(Token *tokens) {
  init_parser(tokens);

  while (current() != NULL) {
    SqlExpr *expr = parse_select_stmt();
    if (expr->type == BAD_EXPR) {
      printf("bad expr");
      return parser->ast;
    }

    add_expr(expr);

    break;
    advance();
  }

  parser_out();

  return parser->ast;
}