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

char *expr_type_to_str(SqlExprType type) {
  switch (type) {
    case EXPR_CREATE_TABLE_STMT: return "create table";
    case EXPR_DROP_TABLE_STMT: return "drop table";
    case EXPR_SELECT_STMT: return "select";
    case EXPR_SELECT_CLAUSE: return "select clause";
    case EXPR_FROM_CLAUSE: return "from clause";
    case EXPR_WHERE_CLAUSE: return "where clause";
    case EXPR_INSERT_STMT: return "insert into";
    case EXPR_BINARY: return "binary expr";
    case EXPR_UNARY: return "unary expr";
    case EXPR_ALIAS: return "alias";
    case EXPR_IDENTIFIER: return "identifier";
    case EXPR_NUMERIC: return "numeric";
    case EXPR_ALL_COLUMNS: return "all columns";
    case BAD_EXPR: return "bad expr";
    default: return "unknown expression type";
  };
}
