#include <stdlib.h>
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