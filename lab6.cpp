#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAXV 10
#define MAXP (MAXV * MAXV)

//печать матрицы 
void printM(int n, int a[MAXV][MAXV]) {
    int i, j;
    printf("Матрица смежности (%d вершин):\n", n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

//печать для дек.произ
void printMBig(int n, int a[MAXP][MAXP]) {
    int i, j;
    printf("Матрица смежности (%d вершин):\n", n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

//ген графа
void genGraph(int n, int a[MAXV][MAXV]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            a[i][j] = 0;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (rand() % 2)
                a[i][j] = a[j][i] = 1;
}

//матрица в список
void matrixToList(int n, int a[MAXV][MAXV], int deg[MAXV], int adj[MAXV][MAXV]) {
    int i, j;
    for (i = 0; i < n; i++)
        deg[i] = 0;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (a[i][j])
                adj[i][deg[i]++] = j;
}
//вывод списка
void printList(int n, int deg[MAXV], int adj[MAXV][MAXV]) {
    int i, k;
    printf("Списки смежности:\n");
    for (i = 0; i < n; i++) {
        printf("Вершина %d: ", i + 1);
        for (k = 0; k < deg[i]; k++)
            printf("%d ", adj[i][k] + 1);
        printf("\n");
    }
    printf("\n");
}

//отождествление
void mergeVertices(int a[MAXV][MAXV], int* n, int v1, int v2) {
    int i;
    if (v1 > v2) { int t = v1; v1 = v2; v2 = t; }

    for (i = 0; i < *n; i++)
        if (a[v2][i])
            a[v1][i] = a[i][v1] = 1;

    a[v1][v1] = 0;

    int last = (*n) - 1;
    if (v2 != last) {
        for (i = 0; i < *n; i++) {
            a[v2][i] = a[last][i];
            a[i][v2] = a[i][last];
        }
    }
    (*n)--;
}

//стягивание ребра
void contractEdge(int a[MAXV][MAXV], int* n, int v1, int v2) {
    mergeVertices(a, n, v1, v2);
}

//расщепление вершины
void splitVertex(int a[MAXV][MAXV], int* n, int v) {
    if (*n >= MAXV) return;

    int oldN = *n;
    int newV = oldN;
    (*n)++;

    int i;
    for (i = 0; i < *n; i++) {
        a[newV][i] = 0;
        a[i][newV] = 0;
    }

    int neigh[MAXV], k = 0;
    for (i = 0; i < oldN; i++)
        if (a[v][i])
            neigh[k++] = i;

    int half = k / 2;

    for (i = half; i < k; i++) {
        int nb = neigh[i];
        a[v][nb] = a[nb][v] = 0;
        a[newV][nb] = a[nb][newV] = 1;
    }

    a[v][newV] = a[newV][v] = 1;
}

//объединение
void unionGraphs(int n, int A[MAXV][MAXV], int B[MAXV][MAXV], int R[MAXV][MAXV]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            R[i][j] = (A[i][j] || B[i][j]);
}

//пересечение
void interGraphs(int n, int A[MAXV][MAXV], int B[MAXV][MAXV], int R[MAXV][MAXV]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            R[i][j] = (A[i][j] && B[i][j]);
}

//кольцевая сумма
void xorGraphs(int n, int A[MAXV][MAXV], int B[MAXV][MAXV], int R[MAXV][MAXV]) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            R[i][j] = (A[i][j] + B[i][j]) % 2;
}

//декартово произведение
void decpro(int n1, int n2,
    int A[MAXV][MAXV], int B[MAXV][MAXV],
    int R[MAXP][MAXP]) {
    int i1, i2, k1, k2;
    int n = n1 * n2;

    for (i1 = 0; i1 < n1; i1++) {
        for (k1 = 0; k1 < n2; k1++) {
            int idx1 = i1 * n2 + k1;
            for (i2 = 0; i2 < n1; i2++) {
                for (k2 = 0; k2 < n2; k2++) {
                    int idx2 = i2 * n2 + k2;
                    int e = 0;

                    if (i1 == i2 && B[k1][k2]) e = 1;
                    if (k1 == k2 && A[i1][i2]) e = 1;

                    R[idx1][idx2] = e;
                }
            }
        }
    }
    for (int i = 0; i < n; i++)
        R[i][i] = 0;
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    int n1, n2;
    int G1[MAXV][MAXV], G2[MAXV][MAXV];

    printf("Введите число вершин графа G1: ");
    scanf("%d", &n1);
    printf("Введите число вершин графа G2: ");
    scanf("%d", &n2);

    genGraph(n1, G1);
    genGraph(n2, G2);

    printf("\nG1:\n");
    printM(n1, G1);
    printf("G2:\n");
    printM(n2, G2);

    int deg1[MAXV], adj1[MAXV][MAXV];
    int deg2[MAXV], adj2[MAXV][MAXV];

    matrixToList(n1, G1, deg1, adj1);
    matrixToList(n2, G2, deg2, adj2);

    printf("Списки смежности G1:\n");
    printList(n1, deg1, adj1);
    printf("Списки смежности G2:\n");
    printList(n2, deg2, adj2);

    int cmd;
    while (1) {
        printf("----- МЕНЮ -----\n");
        printf("1 - Отождествить две вершины G1\n");
        printf("2 - Стянуть ребро G1\n");
        printf("3 - Расщепить вершину G1\n");
        printf("4 - Объединение G1 и G2\n");
        printf("5 - Пересечение G1 и G2\n");
        printf("6 - Кольцевая сумма G1 и G2\n");
        printf("7 - Декартово произведение G1 и G2\n");
        printf("0 - Выход\n");
        printf("Введите пункт меню: ");

        scanf("%d", &cmd);
        if (cmd == 0) break;

        int v1, v2;
        int R[MAXV][MAXV];
        int RP[MAXP][MAXP];

        switch (cmd) {
        case 1:
            printf("Введите номер первой вершины: ");
            scanf("%d", &v1);
            printf("Введите номер второй вершины: ");
            scanf("%d", &v2);
            mergeVertices(G1, &n1, v1 - 1, v2 - 1);
            printM(n1, G1);
            break;

        case 2:
            printf("Введите две вершины, ребро между которыми стягиваем:\n");
            printf("Первая вершина: ");
            scanf("%d", &v1);
            printf("Вторая вершина: ");
            scanf("%d", &v2);
            contractEdge(G1, &n1, v1 - 1, v2 - 1);
            printM(n1, G1);
            break;

        case 3:
            printf("Введите номер вершины для расщепления: ");
            scanf("%d", &v1);
            splitVertex(G1, &n1, v1 - 1);
            printM(n1, G1);
            break;

        case 4:
            if (n1 == n2) {
                unionGraphs(n1, G1, G2, R);
                printM(n1, R);
            }
            break;

        case 5:
            if (n1 == n2) {
                interGraphs(n1, G1, G2, R);
                printM(n1, R);
            }
            break;

        case 6:
            if (n1 == n2) {
                xorGraphs(n1, G1, G2, R);
                printM(n1, R);
            }
            break;

        case 7: {
            int nProd = n1 * n2;
            decpro(n1, n2, G1, G2, RP);
            printMBig(nProd, RP);
            break;
        }
        }
    }

    return 0;
}
