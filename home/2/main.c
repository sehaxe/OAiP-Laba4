#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Задача 2. Вариант 5
// Проверить, есть ли в матрице хотя бы одна строка, содержащая
// положительный элемент, и найти её номер. Знаки элементов предыдущей
// строки изменить на противоположные.

#define MAX_N 100
#define RND_MIN (-50)
#define RND_MAX 50

static int read_int(const char *prompt, const int lo, const int hi, int *value)
{
    char line[128];
    int v;

    for (;;) {
        printf("%s", prompt);
        if (fgets(line, sizeof line, stdin) == NULL) {
            return 0;
        }
        if (sscanf(line, "%d", &v) == 1 && v >= lo && v <= hi) {
            *value = v;
            return 1;
        }
        printf("Ошибка: нужно целое число от %d до %d.\n", lo, hi);
    }
}

static int read_size(int *n, int *m)
{
    char line[128];

    for (;;) {
        printf("Размеры матрицы n m (1..%d): ", MAX_N);
        if (fgets(line, sizeof line, stdin) == NULL) {
            return 0;
        }
        if (sscanf(line, "%d %d", n, m) == 2 &&
            *n >= 1 && *n <= MAX_N && *m >= 1 && *m <= MAX_N) {
            return 1;
        }
        printf("Ошибка: нужно два целых числа от 1 до %d.\n", MAX_N);
    }
}

static int fill_matrix(int a[][MAX_N], const int n, const int m)
{
    char prompt[32];
    int mode, i, j;

    if (!read_int("Заполнение: 1 - вручную, 2 - случайно: ", 1, 2, &mode)) {
        return 0;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (mode == 1) {
                snprintf(prompt, sizeof prompt, "a[%d][%d] = ", i, j);
                if (!read_int(prompt, INT_MIN, INT_MAX, &a[i][j])) {
                    return 0;
                }
            } else {
                a[i][j] = rand() % (RND_MAX - RND_MIN + 1) + RND_MIN;
            }
        }
    }
    return 1;
}

static void print_matrix(const char *title, const int a[][MAX_N],
                         const int n, const int m)
{
    int i, j;

    printf("%s\n", title);
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%5d", a[i][j]);
        }
        printf("\n");
    }
}

// Номер первой строки с положительным элементом или -1, если такой нет.
static int first_row_with_positive(const int a[][MAX_N],
                                   const int n, const int m)
{
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            if (a[i][j] > 0) {
                return i;
            }
        }
    }
    return -1;
}

static void flip_row(int a[][MAX_N], const int m, const int row)
{
    int j;

    for (j = 0; j < m; j++) {
        a[row][j] = -a[row][j];
    }
}

int main(void)
{
    int a[MAX_N][MAX_N];
    int n, m, row;

    if (!read_size(&n, &m)) {
        return 0;
    }
    if (!fill_matrix(a, n, m)) {
        return 0;
    }

    print_matrix("Введённая матрица:", a, n, m);

    row = first_row_with_positive(a, n, m);
    if (row == -1) {
        printf("Строк с положительным элементом нет.\n");
    } else {
        printf("Первая строка с положительным элементом: %d (индекс %d)\n",
               row + 1, row);
        if (row == 0) {
            printf("Предыдущей строки нет, матрица не изменилась.\n");
        } else {
            flip_row(a, m, row - 1);
            printf("Знаки элементов строки %d изменены на противоположные.\n",
                   row);
            print_matrix("Матрица после изменения:", a, n, m);
        }
    }

    return 0;
}
