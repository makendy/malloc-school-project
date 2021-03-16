#ifndef MALLOC_H
#define MALLOC_H

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#define SIZE_OF_PAGE sizeof(struct page)
#define SIZE_OF_META sizeof(struct metadata)

struct metadata
{
    unsigned free;
    size_t size;
    void *data;
    struct metadata *next;
};

struct page
{
    void *page;
    size_t page_size;
    struct metadata *meta_data;
    struct page *next;
};

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);
size_t size_of_page(size_t size);
void *shift_of_addr(void *addr, size_t shift);
struct page *create_page(size_t size);
struct metadata *find_data(void *ptr, struct page *page);

#endif /* !MALLOC_H */