#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Задача 1. Вариант 5
// Дана целочисленная прямоугольная матрица. Определить количество
// столбцов, не содержащих ни одного нулевого элемента.

#define MAX_N 100
#define RND_MIN (-50)
#define RND_MAX 50

// Читает строку и разбирает из неё одно целое число в диапазоне [lo, hi].
// Возвращает 0, если строки кончились (конец файла).
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

// Размеры прямоугольной матрицы: два числа в одной строке.
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

// Заполнение матрицы: 1 — вручную, 2 — случайными числами.
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

// Считает столбцы, в которых нет ни одного нуля.
static int count_cols_without_zero(const int a[][MAX_N],
                                   const int n, const int m)
{
    int count, i, j, has_zero;

    count = 0;
    for (j = 0; j < m; j++) {
        has_zero = 0;
        for (i = 0; i < n; i++) {
            if (a[i][j] == 0) {
                has_zero = 1;
                break;
            }
        }
        if (!has_zero) {
            count++;
        }
    }
    return count;
}

int main(void)
{
    int a[MAX_N][MAX_N];
    int n, m, count;

    if (!read_size(&n, &m)) {
        return 0;
    }
    if (!fill_matrix(a, n, m)) {
        return 0;
    }

    print_matrix("Введённая матрица:", a, n, m);

    count = count_cols_without_zero(a, n, m);
    printf("Столбцов без нулевых элементов: %d\n", count);

    return 0;
}
