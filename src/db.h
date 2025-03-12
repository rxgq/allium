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

typedef struct {
  Database *db;
  int debug;
  char *err;
} AlliumDb;

extern AlliumCode execute(AlliumDb *allium, SqlQueryTree *ast);
extern AlliumDb *init_allium(int debug);

#endif