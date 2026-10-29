/* Упрощённая схема алгоритма задания 2: блоки проверки ввода не показаны.
   Полная программа — ../2/main.c. */
#include <stdio.h>
#define MAX_N 100
int main(void) {
    int a[MAX_N][MAX_N];
    int n, m, i, j, row;
    scanf("%d%d", &n, &m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &a[i][j]);
    row = -1;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (a[i][j] > 0 && row == -1) {
                row = i;
            }
        }
    }
    if (row == -1) {
        printf("нет");
    } else {
        if (row > 0) {
            for (j = 0; j < m; j++) {
                a[row - 1][j] = -a[row - 1][j];
            }
        }
        printf("row = %d", row);
    }
    return 0;
}
