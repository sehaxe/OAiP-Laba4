#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Задача 3. Вариант 5
// В квадратной матрице размером NxN найти максимальный элемент
// в 1-ой области (верхний треугольник между главной и побочной
// диагоналями).

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

// Размер квадратной матрицы.
static int read_size(int *n)
{
    char line[128];

    for (;;) {
        printf("Размер матрицы n (1..%d): ", MAX_N);
        if (fgets(line, sizeof line, stdin) == NULL) {
            return 0;
        }
        if (sscanf(line, "%d", n) == 1 && *n >= 1 && *n <= MAX_N) {
            return 1;
        }
        printf("Ошибка: нужно целое число от 1 до %d.\n", MAX_N);
    }
}

static int fill_matrix(int a[][MAX_N], const int n)
{
    char prompt[32];
    int mode, i, j;

    if (!read_int("Заполнение: 1 - вручную, 2 - случайно: ", 1, 2, &mode)) {
        return 0;
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
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

static void print_matrix(const char *title, const int a[][MAX_N], const int n)
{
    int i, j;

    printf("%s\n", title);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%5d", a[i][j]);
        }
        printf("\n");
    }
}

// Максимум в области 1: элементы строго выше обеих диагоналей,
// то есть j > i и j < n - 1 - i. found = 0, если область пуста.
static int max_region1(const int a[][MAX_N], const int n, int *found)
{
    int best, i, j;

    best = 0;
    *found = 0;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n - 1 - i; j++) {
            if (!*found || a[i][j] > best) {
                best = a[i][j];
                *found = 1;
            }
        }
    }
    return best;
}

int main(void)
{
    int a[MAX_N][MAX_N];
    int n, found, best;

    if (!read_size(&n)) {
        return 0;
    }
    if (!fill_matrix(a, n)) {
        return 0;
    }

    print_matrix("Введённая матрица:", a, n);

    best = max_region1(a, n, &found);
    if (!found) {
        printf("Область 1 пуста: в матрице меньше трёх строк.\n");
    } else {
        printf("Максимальный элемент в области 1: %d\n", best);
    }

    return 0;
}
