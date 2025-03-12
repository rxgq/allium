#include <stdlib.h>
#include <stdio.h>
#include "expr.h"
#include "db.h"

static Database *init_db() {
  Database *db = malloc(sizeof(Database));
  db->table_count = 0;
  db->tables = malloc(sizeof(Table) * MAX_TABLES);

  return db;
}


void execute_create_table(SqlExpr *expr) {
  CreateTableStmt *create_table = &expr->as.create_table;

  printf(create_table->name->as.identifier.value);

  
}

void execute(SqlQueryTree *ast) {
  Database *db = init_db();

  for (int i = 0; i < ast->statement_count; i++) {
    execute_create_table(&ast->statements[i]);
    break;
  }
}