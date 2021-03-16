#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
    char *str1;
    char *str2 = malloc(sizeof(char) * 12);
    void *p1 = str2;
    free(str2);
    char *str3 = malloc(sizeof(char) * 12);
    void *p2 = str3;
    free(str3);
    assert(p1 == p2);
    free(str1);
    return 0;
}