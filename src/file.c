#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file.h"

char *read_file(const char *path) {
  FILE *fptr = fopen(path, "r");
  if (!fptr) {
    printf("invalid file path.\n");
    return NULL;
  }

  fseek(fptr, 0, SEEK_END);
  unsigned long sz = ftell(fptr);
  rewind(fptr);

  char *buff = malloc(sz + 1);
  if (!buff) {
    printf("Memory allocation error.\n");
    fclose(fptr);
    return NULL;
  }

  size_t read_size = fread(buff, 1, sz, fptr);
  buff[read_size] = '\0';

  fclose(fptr);
  return buff;
}

int has_sql_extension(const char *filename) {
  const char *ext = strrchr(filename, '.');
  return ext && strcmp(ext, ".sql") == 0;
}