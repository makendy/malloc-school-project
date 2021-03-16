#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
    char *str1;
    char *str2 = calloc(12, sizeof(char));
    for (int i = 0; i < 12; i++)
        printf("%c", str2[i]);
    puts("");
    free(str2);
    char *str3 = calloc(12, sizeof(int));
    for (int i = 0; i < 12; i++)
        printf("%d", str3[i]);
    puts("");
    char *str4 = calloc(12, sizeof(double));
    for (int i = 0; i < 12; i++)
        printf("%d", str4[i]);
    puts("");
    char *str5 = calloc(0, sizeof(double));
    puts("");
    free(str3);
    free(str4);
    free(str1);
    return 0;
}