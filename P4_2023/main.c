#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define TAM_MAX 1000

typedef int ** matriz;




void dijkstra(int n, matriz grafo, matriz distancias) {
    int m, i, v, w;

    for (m = 0; m < n; m++) {
        int noVisitados[TAM_MAX];
        for (i = 0; i < n; i++) {
            distancias[m][i] = grafo[m][i];
            noVisitados[i] = 1;
        }
        noVisitados[m] = 0;

        for (i = 0; i < n - 2; i++) {
            v = -1;
            for (int j = 0; j < n; j++) {
                if (noVisitados[j] && (v == -1 || distancias[m][j] < distancias[m][v])) {
                    v = j;
                }
            }

            noVisitados[v] = 0;

            for (w = 0; w < n; w++) {
                if (noVisitados[w] && distancias[m][w] > distancias[m][v] + grafo[v][w]) {
                    distancias[m][w] = distancias[m][v] + grafo[v][w];
                }
            }
        }
    }
}

matriz crearMatriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

void iniMatriz(matriz m, int n) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % TAM_MAX + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}

void liberarMatriz(matriz m, int n) {
    int i;
    for (i=0; i<n; i++){
        free(m[i]);
    }
    free(m);
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void matrEj(matriz m, int n) {
    int i,j;
    int Matr5 [5] [5] = {{0, 1, 8, 4, 7},
                         {1, 0, 2, 6, 5},
                         {8, 2, 0, 9, 5},
                         {4, 6, 9, 0, 3},
                         {7, 5, 5, 3, 0}};

    int Matr4 [4] [4] ={{0, 1, 4, 7},
                        {1, 0, 2, 8},
                        {4, 2, 0, 3},
                        {7, 8, 3, 0}};
    if (n==5) {
        for (i=0; i<n; i++) {
            for (j=0; j<n; j++) {
                m[i][j]=Matr5[i][j];
            }
        }
    }else {
        for (i=0; i<n; i++) {
            for (j=0; j<n; j++) {
                m[i][j]=Matr4[i][j];
            }
        }
    }



}

void imprTest (bool igual, matriz mat, matriz dist, int n) {
    int i, j;

    if(n==5){
        printf("\nMatriz Base\n");
        for (i=0; i<5; i++) {
            for (j=0;j<5;j++) {
                printf("%d  ",mat[i][j]);
            }
            printf("\n");
        }
        printf("\nMatriz Distancias\n");
        for (i=0; i<5; i++) {
            for (j=0;j<5;j++) {
                printf("%d  ",dist[i][j]);
            }
            printf("\n");
        }
    } else{
        printf("\nMatriz Base\n");
        for (i=0; i<4; i++) {
            for (j=0;j<4;j++) {
                printf("%d  ",mat[i][j]);
            }
            printf("\n");
        }
        printf("\nMatriz Distancias\n");
        for (i=0; i<4; i++) {
            for (j=0;j<4;j++) {
                printf("%d  ",dist[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");
    if(igual==true){
        printf("Exitoso\n");
    }else{
        printf("Fallido\n");
    }
}

void test1 () {
    int i, j;
    bool iguales1=true, iguales2=true;
    int Dist1 [5] [5] = {{0, 1, 3, 4, 6}, {1, 0, 2, 5, 5},
                         {3, 2, 0, 7, 5}, {4, 5, 7, 0, 3}, {6, 5, 5, 3, 0}};

    int Dist2 [4] [4] = {{0, 1, 3, 6}, {1, 0, 2, 5},
                         {3, 2, 0, 3}, {6, 5, 3, 0}};

    matriz m1 = crearMatriz(5);
    matriz m2 = crearMatriz(4);
    matriz d1 = crearMatriz(5);
    matriz d2 = crearMatriz(4);

    matrEj(m1,5);
    matrEj(m2,4);

    dijkstra(5,m1,d1);
    dijkstra(4,m2,d2);

    for (i=0; i<5; i++) {
        for (j=0;j<5;j++) {
            if (d1[i][j] != Dist1[i][j]){
                iguales1=false;
            }
        }
    }
    for (i=0; i<4; i++) {
        for (j=0;j<4;j++) {
            if (d2[i][j] != Dist2[i][j]){
                iguales2=false;
            }
        }
    }
    imprTest(iguales1,m1,d1,5);
    imprTest(iguales2,m2,d2,4);
    liberarMatriz(m1,5);
    liberarMatriz(m2,4);
    liberarMatriz(d1,5);
    liberarMatriz(d2,4);
}

double calc_tmp_medio(matriz m, matriz distancias,int n){
    double tiempo_inicio=0, tiempo_final=0, t1=0;

    tiempo_inicio = microsegundos();
    m = crearMatriz(n);
    iniMatriz(m,n);
    distancias = crearMatriz(n);
    tiempo_final = microsegundos();
    t1 = (tiempo_final - tiempo_inicio);

    dijkstra(n,m,distancias);
    liberarMatriz(m,n);
    liberarMatriz(distancias,n);
    return t1;
}

void tiempos(){
    double tiempo_inicio = 0, tiempo_final = 0, t = 0, tiempoDeCreaciones = 0;
    int n, cont;
    matriz m;
    matriz distancias;

    printf("\n\t n\t\t\tt(n)\t\tt(n)/n^2.8\t    t(n)/n^2.9\t         ");
    printf("t(n)/n^3.0\n");

    for (n = 10; n <= TAM_MAX && t < 500000; n = 2*n) {
        m = crearMatriz(n);
        distancias = crearMatriz(n);
        iniMatriz(m,n);

        tiempo_inicio = microsegundos();
        dijkstra(n,m,distancias);
        tiempo_final = microsegundos();
        t = tiempo_final - tiempo_inicio;
        liberarMatriz(m,n);
        liberarMatriz(distancias,n);
        if(t<500){
            tiempo_inicio= microsegundos();
            for (cont=0; cont<1000; cont++) {
                tiempoDeCreaciones += calc_tmp_medio(m, distancias, n);
            }
            tiempo_final = microsegundos();
            t = ((tiempo_final- tiempoDeCreaciones) - tiempo_inicio)/1000;

            printf("(*)%7d\t%20.5f%20.6f%20.6f%20.6f\n",n,t,(t/(pow(n, 2.8))),
                   (t/(pow(n, 2.9))),(t/(pow(n, 3))));

        }else {
            printf("%10d\t%20.5f%20.6f%20.6f%20.6f\n",n,t,
                   ((double)t/(pow(n, 2.8))),((double)t/(pow(n, 2.9))),
                   ((double)t/(pow(n, 3))));
        }
    }
}

int main (){
    test1();
    tiempos();
    tiempos();
    tiempos();
}