#ifndef PAGE_H
#define PAGE_H

#define PAGE_SIZE 4096
#define MAX_PAGES 32

typedef struct {
  int page_id;
  char data[PAGE_SIZE];
} Page;

typedef struct {
  Page *pages[MAX_PAGES];
  int page_count;
} PageBuffer;

#endif