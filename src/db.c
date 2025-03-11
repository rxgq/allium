// #include "db.h"
// #include <stdlib.h>
// #include <stdio.h>

// #include "db.h"
// #include "expr.h"

// Database *init_db() {
//   Database *db = malloc(sizeof(Database));
//   db->table_count = 0;
//   db->tables = malloc(sizeof(Table) * MAX_TABLES);

//   return db;
// }

// void execute_create_table(SqlExpr *expr) {
//   CreateTableStmt *create_table = &expr->as.create_table;

//   printf("creating table: ");
//   printf(create_table->name->as.identifier.value);
// }

// void execute(SqlQueryTree *ast) {
//   Database *db = init_db();
//   printf("test");

//   for (int i = 0; i < ast->statement_count; i++) {
//     execute_create_table(&ast->statements[i]);
//     break;
//   }
// }