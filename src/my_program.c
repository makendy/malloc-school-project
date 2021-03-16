#include "malloc.h"

static void *my_get_page(size_t g_ressource)
{
    void *addr = mmap(NULL, g_ressource + SIZE_OF_META, PROT_READ
        | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
        return NULL;
    return addr;
}

size_t size_of_page(size_t size)
{
    size_t g_ressource;
    long tmp = sysconf(_SC_PAGE_SIZE);
    if (tmp == -1)
        g_ressource = 4096;
    else
        g_ressource = tmp;
    while (g_ressource < size)
        g_ressource += g_ressource;
    return g_ressource;
}

void *shift_of_addr(void *addr, size_t shift)
{
    if (!addr)
        return NULL;
    void *res = (char*)addr + shift;
    return res;
}

struct page *create_page(size_t size)
{
    size_t g_ressource = size_of_page(size);
    struct page *page = my_get_page(g_ressource);
    page->page = page;
    if (page->page == NULL)
        return NULL;
    page->meta_data = shift_of_addr(page->page,SIZE_OF_PAGE);
    page->page_size = g_ressource - SIZE_OF_PAGE;
    page->meta_data->free = 0;
    page->meta_data->size = size;
    page->meta_data->data = shift_of_addr(page->page,SIZE_OF_PAGE
        + SIZE_OF_META);
    page->page_size -= SIZE_OF_META;
    page->meta_data->next = shift_of_addr(page->page,SIZE_OF_PAGE
        + SIZE_OF_META + page->meta_data->size);
    page->page_size -= page->meta_data->size;
    page->meta_data->next->free = 1;
    page->meta_data->next->size = 0;
    page->meta_data->next->next = NULL;
    page->next = NULL;
    return page;
}

struct metadata *find_data(void *ptr, struct page *page)
{
    if (!ptr)
        return NULL;
    struct page *tmp = page;
    while (tmp)
    {
        struct metadata *meta = tmp->meta_data;
        while (meta)
        {
            if (meta->data == ptr)
                return meta;
            meta = meta->next;
        }
        tmp = tmp->next;
    }
    return NULL;
}