#ifndef DB_H
#define DB_H

#include "table.h"
#include "expr.h"
#include "codes.h"

#define MAX_TABLES 32

typedef struct {
  int table_count;
  Table *tables;
} Database;

extern AlliumCode execute(Database *db, SqlQueryTree *ast);
extern Database *init_db();

#endif