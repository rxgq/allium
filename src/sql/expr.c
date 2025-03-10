#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "expr.h"

SqlExpr *init_expr(SqlExprType type) {
  SqlExpr *expr = malloc(sizeof(SqlExpr));
  expr->type = type;

  return expr;
}

SqlQueryTree *init_sql_tree() {
  SqlQueryTree *tree = malloc(sizeof(SqlQueryTree));
  tree->statement_capacity = 1;
  tree->statement_count = 0;
  tree->statements = malloc(sizeof(SqlExpr) * tree->statement_capacity);

  return tree;
}

void print_depth(int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
}

void print_expr(SqlExpr *expr, int depth) {
  print_depth(depth);

  switch (expr->type) {
    case EXPR_SELECT_STMT:
      printf("SELECT\n");
      for (int i = 0; i < 2; i++) {
        print_expr(&expr->as.select.clauses[i], depth + 1);
      }
      return;

    case EXPR_SELECT_CLAUSE:
      printf("SELECT CLAUSE:\n");
      for (int i = 0; i < expr->as.select_clause.options_count; i++) {
        print_expr(&expr->as.select_clause.options[i], depth + 1);
      }
      return;

    case EXPR_IDENTIFIER:
      printf("IDENTIFIER: %s\n", expr->as.identifier.value);
      return;

    case EXPR_FROM_CLAUSE:
      printf("FROM CLAUSE:\n");
      print_expr(expr->as.from_clause.expr, depth + 1);
      return;

    case EXPR_ALIAS:
      printf("ALIAS:\n");
      print_expr(expr->as.alias.expr, depth + 1);
      print_expr(expr->as.alias.identifier, depth + 1);
      return;

    case EXPR_ALL_COLUMNS:
      printf("ALL COLUMNS: *\n");
      return;

    case EXPR_CREATE_TABLE_STMT:
      printf("CREATE TABLE\n");
      for (int i = 0; i < expr->as.create_table.column_count; i++) {
        print_depth(depth + 1);
        printf("%s %s\n", expr->as.create_table.columns[i].type, expr->as.create_table.columns[i].name);
      }
      return;

    case BAD_EXPR:
      printf("BAD EXPR\n");
      return;
      
    default: 
      printf("Unknown type '%d'\n", expr->type);
  }
}

void parser_out(ParserState *parser) {
  printf("\n\nSQL Parse Tree:\n");
  for (int i = 0; i < parser->ast->statement_count; i++) {
    print_expr(&parser->ast->statements[i], 1);
  }
  printf("\n\n");
}