#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

void inicializar_semilla(){
    srand(time(NULL));
}

void printVector(int v[], int n){
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d\t", v[i]);
    }
    printf("\n");
}

void aleatorio(int v[], int n){
    int i, m = 2 * n + 1;
    for(i = 0; i < n; i++){
        v[i] = (rand() % m) - n;
    }
}

void ascendente(int v[], int n){
    int i;
    for(i = 0; i < n; i++){
        v[i] = i;
    }
}

void descendente(int v[], int n){
    int i;
    for(i = 0; i < n; i++){
        v[i] = n - i;
    }
}

void intercambiar(int *n1, int *n2){
    int x;
    x = *n1;
    *n1 = *n2;
    *n2 = x;
}

void ord_ins(int v[], int n){
    int i, j, x;
    for (i = 1; i < n; ++i) {
        x = v[i];
        j = i-1;
        while (j>=0 && v[j]>x){
            v[j+1] = v[j];
            j= j-1;
        }
        v[j+1] = x;
    }
}

void ord_rap_aux(int v[], int iz, int dr){
    int x, pivote, i, j;
    if(iz < dr){
        x = iz + rand() % (dr-iz+1);
        pivote = v[x];
        intercambiar(&v[iz], &v[x]);
        i = iz + 1;
        j = dr;
        while(i<=j){
            while(i<=dr && v[i] < pivote){
                i = i+1;
            }
            while (v[j] > pivote){
                j = j-1;
            }
            if(i<=j){
                intercambiar(&v[i], &v[j]);
                i = i+1;
                j = j-1;
            }
        }
        intercambiar(&v[iz], &v[j]);
        ord_rap_aux(v, iz, j-1);
        ord_rap_aux(v, j+1, dr);
    }
}

void ord_rap(int v[], int n){
    ord_rap_aux(v, 0, n-1);
}

int orden(int v[], int n){
    int i;
    for (i = 1; i < n; ++i) {
        if(v[i]<v[i-1]){
            return 0;
        }
    }
    return 1;
}

void test(int valor){
    if(valor==1){
        printf("El vector esta ordenado.\n");
    }else{
        printf("EL vector no esta ordenado.\n");
    }
}

void cabeceras(int ini, int ord){
    printf("Ordenacion por ");
    if(ord == 1){
        printf("insercion con inicializacion ");
        if(ini == 1){
            printf("aleatoria");
        }else if(ini == 2){
            printf("descendente");
        }else{
            printf("ascendente");
        }
    }else{
        printf("quickort con inicializacion ");
        if(ini == 1){
            printf("aleatoria");
        }else if(ini == 2){
            printf("descendente");
        }else{
            printf("ascendente");
        }
    }
    printf(":\n");
}

double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0){
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void inicializaciones(int v[], int n, int ini){
    if(ini==1){
        aleatorio(v, n);
    }else if(ini==2){
        descendente(v, n);
    }else if(ini==3){
        ascendente(v, n);
    }else{
        printf("EL valor recibido como clave de inicializacion es incorrecto.\n");
    }
}

void ordenaciones(int v[], int n, int ord){
    if(ord==1){
        ord_ins(v, n);
    }else if(ord==2){
        ord_rap(v, n);
    }else{
        printf("El valor recibido como clave de ordenaciones es incorrecto.\n");
    }
}

double tpeque(int v[], int n, int ord, int ini){
    double tiempo_inicio=0, tiempo_final=0, t1=0, t2=0, tfin=0;
    int cont;
    if (ord == 1) {
        tiempo_inicio = microsegundos();
        for(cont=0; cont<1000; cont++) {
            inicializaciones(v, n, ini);
            ord_ins(v,n);
        }
        tiempo_final = microsegundos();
    }else if (ord == 2) {
        tiempo_inicio = microsegundos();
        for(cont=0; cont<1000; cont++) {
            ord_rap(v,n);
            inicializaciones(v, n, ini);
        }
        tiempo_final = microsegundos();
    }

    t1 = (tiempo_final - tiempo_inicio);
    tiempo_inicio= microsegundos();
    for(cont=0; cont<1000; cont++) {
        inicializaciones(v, n, ini);
    }
    tiempo_final = microsegundos();
    t2 = tiempo_final-tiempo_inicio;

    tfin = (t1-t2)/1000;

    return tfin;
}

double calc_tmp(int v[], int n, int opcion_ord){
    double tiempo_inicio=0, tiempo_final=0, t=0;
    if (opcion_ord == 1) {
        tiempo_inicio = microsegundos();
        ord_ins(v,n);
        tiempo_final = microsegundos();
    }else if (opcion_ord == 2) {
        tiempo_inicio = microsegundos();
        ord_rap(v,n);
        tiempo_final = microsegundos();
    }

    t = tiempo_final - tiempo_inicio;

    return t;
}

void cotas (int ini, int ord, double t, int n) {
    if (ord == 1) {
        if (ini == 1) { //insercion aleatorio
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.1))));
        } else if (ini == 2){ //insercion descendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.2))));
        } else if (ini == 3){ //insercion ascendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.8))),(t/(pow(n, 1.0))),(t/(pow(n, 1.2))));
        }
    }else if (ord == 2) {
        if (ini == 1) { //quicksort aleatorio
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(n* log(n))),(t/(pow(n, 2))));
        } else if (ini == 2){ //quicksort descendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 2))),(t/(pow(n, 2)* log(n))));
        } else if (ini == 3){ //quicksort ascendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(n* log(n))),(t/(pow(n, 2))));
        }
    }
}

void cotas_s (int ini, int ord, double t, int n) {
    if (ord == 1) {
        if (ini == 1) { //insercion aleatorio
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.2))));
        } else if (ini == 2){ //insercion descendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.2))));
        } else if (ini == 3){ //insercion ascendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.8))),(t/(pow(n, 1.0))),(t/(pow(n, 1.2))));
        }
    }else if (ord == 2) {
        if (ini == 1) { //quicksort aleatorio
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t, (t / (n)) , (t / (n* log(n))), (t/(n*n)));
        } else if (ini == 2){ //quicksort descendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(n)),(t/(n*n)),(t/((n*n)* log(n))));
        } else if (ini == 3){ //quicksort ascendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(n* log(n))),(t/(pow(n, 2))));
        }
    }
}

void indicar_cota(int ord, int ini){
    if(ord==1){
        if(ini==1){
            printf("\t\t\tCota subestimada: n ^ 1.8\tCota ajustada: n ^ 2\tCota sobrestimada: n ^ 2.2\n");
        }else if(ini==2){
            printf("\t\t\tCota subestimada: n ^ 1.8\tCota ajustada: n ^ 1.8\tCota sobrestimada: n ^ 2\n");
        }else{
            printf("\t\t\tCota subestimada: n ^ 0.8\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.2\n");
        }
    }else if(ord==2){
        if(ini==1){
            printf("\t\t\tCota subestimada: n\tCota ajustada: n * log(n)\tCota sobrestimada:n ^ 2\n");
        }else if(ini==2){
            printf("\t\t\tCota subestimada: n\tCota ajustada: n ^ 2\tCota sobrestimada: n ^ 2 * log(n)\n");
        }else{
            printf("\t\t\tCota subestimada: n\tCota ajustada: n * log(n)\tCota sobrestimada: n ^ 2\n");
        }
    }

}

void tiempo(int ini, int ord) {
    int v[32000];
    double t;
    int n;
    printf("Tiempos y complejidades computacionales del algoritmo:\n");
    cabeceras(ini, ord);
    for (n = 500; n <= 32000; n *= 2) {
        inicializaciones(v, n, ini);
        t = calc_tmp(v, n, ord);
        if (t < 500) {
            t = tpeque(v, n, ord, ini);
            cotas_s(ini, ord, t, n);
        } else {
            cotas(ini, ord, t, n);
        }
    }
    indicar_cota(ord, ini);
}

void tiempos() {
    int i, j;
    for (i = 1; i <= 2; i++) {
        for (j = 1; j <= 3; j++) {
            tiempo(j, i);
            printf("\n");
        }
    }
}

void TEST(int v[], int n){
    int i, j;
    for (i = 1; i <= 2; i++) {
        for (j = 1; j <= 3; j++) {
            cabeceras(j, i);
            inicializaciones(v, n, j);
            printVector(v, n);
            test(orden(v, n));
            ordenaciones(v,n, i);
            printVector(v, n);
            test(orden(v, n));
            printf("\n");
        }
    }
}

int main(void) {
    inicializar_semilla();
    int n = 17, v[n];
    TEST(v, n);
    tiempos();
    return 0;
}