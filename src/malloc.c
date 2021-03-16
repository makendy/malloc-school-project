#include "malloc.h"
struct page *available_page = NULL;

static void *first_fit_block(struct page *page, size_t size)
{
    struct metadata *tmp;
    tmp = page->meta_data;
    struct metadata *save;
    size_t shift = SIZE_OF_PAGE;
    while (tmp != NULL)
    {
        shift += tmp->size + SIZE_OF_META;
        save = tmp;
        if (save->free == 1 && save->size >= size)
            return save->data;
        tmp = tmp->next;
    }
    if (save->free == 0)
        return NULL;
    save->free = 0;
    if (save->size < size)
        save->size = size;
    save->data = shift_of_addr(page->page,shift + SIZE_OF_META);
    page->page_size -= SIZE_OF_META;
    save->next = shift_of_addr(page->page,shift + SIZE_OF_META
    + save->size);
    page->page_size -= save->size;
    save->next->free = 1;
    save->next->size = 0;
    save->next->next = NULL;
    return save->data;
}

__attribute__((visibility("default")))
void *malloc(size_t __attribute__((unused)) size)
{
    if (available_page == NULL)
    {
        available_page = create_page(size + SIZE_OF_META
        + SIZE_OF_PAGE);
        if (available_page == NULL)
            return NULL;
        return available_page->meta_data->data;
    }

    struct page *tmp = available_page;
    struct page *save;
    while (tmp != NULL)
    {
        if ((size + SIZE_OF_META) < tmp->page_size)
        {
            void *ret = first_fit_block(tmp, size);
            if (ret)
                return ret;
        }
        save = tmp;
        tmp = tmp->next;
    }
    save->next = create_page(size + SIZE_OF_META
    + SIZE_OF_PAGE);
    if (save->next == NULL)
        return NULL;
    return save->next->meta_data->data;
}

__attribute__((visibility("default")))
void free(void __attribute__((unused)) *ptr)
{
    if (!ptr)
        return;
    struct page *tmp = available_page;
    while (tmp)
    {
        struct metadata *meta;
        meta = tmp->meta_data;
        while (meta)
        {
            if (meta->data == ptr)
            {
                meta->free = 1;
                tmp->page_size += meta->size + SIZE_OF_META;
                return;
            }
            meta = meta->next;
        }
        tmp = tmp->next;
    }
}

__attribute__((visibility("default")))
void *realloc(void __attribute__((unused)) *ptr,
             size_t __attribute__((unused)) size)
{
    if (!ptr)
        return malloc(size);
    if (ptr && size == 0)
    {
        free(ptr);
        return NULL;
    }
    struct metadata *meta = find_data(ptr,available_page);
    if (!meta)
        return NULL;
    if (meta->size <= size)
        return meta->data;

    void *ptr1 = malloc(size);
    if (!ptr1)
        return ptr;
    memcpy(ptr1,ptr,meta->size);
    free(ptr);
    return ptr1;
}

__attribute__((visibility("default")))
void *calloc(size_t __attribute__((unused)) nmemb,
             size_t __attribute__((unused)) size)
{
    if (nmemb == 0 || size == 0)
        return NULL;
    void *ptr = malloc(size * nmemb);
    ptr = memset(ptr, 0,nmemb);
    return ptr;
}
