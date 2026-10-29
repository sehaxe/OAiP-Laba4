/* Упрощённая схема алгоритма задания 3: блоки проверки ввода не показаны.
   Полная программа — ../3/main.c. */
#include <stdio.h>
#define MAX_N 100
int main(void) {
    int a[MAX_N][MAX_N];
    int n, i, j, best, found;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d", &a[i][j]);
    best = 0;
    found = 0;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n - 1 - i; j++) {
            if (found == 0 || a[i][j] > best) {
                best = a[i][j];
                found = 1;
            }
        }
    }
    if (found == 0) printf("пусто");
    else printf("%d", best);
    return 0;
}
