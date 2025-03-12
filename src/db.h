#ifndef DB_H
#define DB_H

#include "table.h"
#include "expr.h"

#define MAX_TABLES 32

typedef struct {
  int table_count;
  Table *tables;
} Database;

extern void execute(SqlQueryTree *ast);

#endif