/* Упрощённая схема алгоритма задания 1: блоки проверки ввода не показаны.
   Полная программа — ../1/main.c. */
#include <stdio.h>
#define MAX_N 100
int main(void) {
    int a[MAX_N][MAX_N];
    int n, m, i, j, count, zero;
    scanf("%d%d", &n, &m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &a[i][j]);
    count = 0;
    for (j = 0; j < m; j++) {
        zero = 0;
        for (i = 0; i < n; i++)
            if (a[i][j] == 0) zero = 1;
        if (zero == 0) count++;
    }
    printf("%d", count);
    return 0;
}
