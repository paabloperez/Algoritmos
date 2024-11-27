#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define TAM 256000
struct monticulo {
    int ultimo;
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;

void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void ascendente(int v[], int n){
    int i;
    for (i=0; i<=n-1; i++)
        v[i] = i+1;
}

void descendente(int v[], int n){
    int valor = n;
    int i;
    for (i = 0; i<n; i++){
        v[i] = valor;
        valor--;
    }
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


void Inicializar_Monticulo(pmonticulo M){
    M->ultimo=-1;
}

int Monticulo_Vacio(pmonticulo M){
    return M->ultimo=-1;
}

void Hundir(pmonticulo M, int i) {
    int HijoIzq, HijoDer, aux, j;

    do {
        HijoIzq = 2 * i + 1;
        HijoDer = 2 * i + 2;
        j = i;

        if (HijoIzq <= M->ultimo && M->vector[HijoIzq] < M->vector[i]) {
            i = HijoIzq;
        }
        if (HijoDer <= M->ultimo && M->vector[HijoDer] < M->vector[i]) {
            i = HijoDer;
        }
        aux = M->vector[j];
        M->vector[j] = M->vector[i];
        M->vector[i] = aux;

    } while (j != i);
}


void crearMonticulo(int v [], int n, pmonticulo M, int opcion_ini){
    Inicializar_Monticulo(M);
    if (opcion_ini == 1) {
        ascendente(v,n);
    } else if (opcion_ini == 2) {
        descendente(v,n);
    } else if (opcion_ini == 3) {
        aleatorio(v,n);
    }
    int i;
    for(i=0 ; i<n ; i++){
        M->vector[i] = v[i];
    }
    M->ultimo = n - 1;
    for(i=(M->ultimo/2) ; i >= 0 ; i--){
        Hundir(M, i);
    }
}

int eliminar_menor(pmonticulo M) {
    int x;
    x = M->vector[0];
    M->vector[0] = M->vector[M->ultimo];
    M->ultimo--;
    if (M->ultimo > 0) {
        Hundir(M, 0);
    }
    return x;
}

void ord_monticulo(int v[], int n, pmonticulo M, int opcion_ini){
    int i;
    crearMonticulo(v,n,M, opcion_ini);
    for (i = 0; i < n ; i++) {
        v[i] = eliminar_menor(M);
    }
}


int tiempo_monticulo(int v[],int n, pmonticulo M){
    double tiempo_inicio, tiempo_final, tiempo_ejecucion;

    tiempo_inicio=microsegundos();
    Inicializar_Monticulo(M);
    crearMonticulo(v,n,M,3);
    tiempo_final=microsegundos();
    tiempo_ejecucion = tiempo_final - tiempo_inicio;

    return tiempo_ejecucion;
}

double calc_tmp(int v[], int n, pmonticulo monticulo, int opcion_ini){
    double tiempo_inicio=0, tiempo_final=0, t=0;

    tiempo_inicio = microsegundos();
    ord_monticulo(v,n, monticulo, opcion_ini);
    tiempo_final = microsegundos();

    t = tiempo_final - tiempo_inicio;

    return t;
}

void inicializaciones (int opcion_ini, int v[], int n) {
    if (opcion_ini == 1) {
        aleatorio(v,n);
    } else if (opcion_ini == 2){
        descendente(v,n);
    } else if (opcion_ini == 3) {
        ascendente(v,n);
    }
}

double calc_tmp_medio(int opcion_ini, int v[], int n, pmonticulo monticulo){
    double tiempo_inicio=0, tiempo_final=0, t1=0, t2=0, tfin=0;
    int cont;

    tiempo_inicio = microsegundos();
    for(cont=0; cont<1000; cont++) {
        inicializaciones(opcion_ini, v, n);
        ord_monticulo(v,n,monticulo,opcion_ini);
    }
    tiempo_final = microsegundos();

    t1 = (tiempo_final - tiempo_inicio);

    tiempo_inicio= microsegundos();
    for(cont=0; cont<1000; cont++) {
        inicializaciones(opcion_ini, v, n);
    }
    tiempo_final = microsegundos();
    t2 = tiempo_final-tiempo_inicio;

    tfin = (t1-t2)/1000;

    return tfin;
}


void cotas (int opcion_ini, double t, int n) {

    if (opcion_ini == 1) { //inicializacion aleatorio
        printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.9))),
               (t/(pow(n, 1.08))),(t/(pow(n, 1.3))));
    } else if (opcion_ini == 2){ //inicializacion descendente
        printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.9))),
               (t/(pow(n, 1.07))),(t/(pow(n, 1.3))));
    } else if (opcion_ini == 3){ //inicializacion ascendente
        printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.9))),
               (t/(pow(n, 1.1))),(t/(pow(n, 1.3))));
    } else if (opcion_ini == 4) { //Creacion de monticulo
        printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.8))),
               (t/(pow(n, 1.0))),(t/(pow(n, 1.2))));
    }

}

void cotas_s (int opcion_ini, double t, int n) {

    if (opcion_ini == 1) { //inicializacion ascendente
        printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.9))),
               (t/(pow(n, 1.08))),(t/(pow(n, 1.3))));
    } else if (opcion_ini == 2){ //inicializacion descendente
        printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.9))),
               (t/(pow(n, 1.07))),(t/(pow(n, 1.3))));
    } else if (opcion_ini == 3){ //inicializacion aleatorio
        printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.9))),
               (t/(pow(n, 1.1))),(t/(pow(n, 1.3))));
    } else if (opcion_ini == 4) { //Creacion de monticulo
        printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.8))),
               (t/(pow(n, 1.0))),(t/(pow(n, 1.2))));
    }

}

void cabeceras(int opcion_ini) {


    if (opcion_ini == 1) { //inicializacion ascendente
        printf("\n\t n\t\t\tt(n)\t      t(n)/n^0.9\t t(n)/n^1.08\t      ");
        printf("t(n)/n^1.3\n");
    } else if (opcion_ini == 2) { //inicializacion descendente
        printf("\n\t n\t\t\tt(n)\t      t(n)/n^0.9\t t(n)/n^1.07\t      ");
        printf("t(n)/n^1.3\n");
    } else if (opcion_ini == 3) { //inicializacion aleatorio
        printf("\n\t n\t\t\tt(n)\t      t(n)/n^0.9\t  t(n)/n^1.1\t      ");
        printf("t(n)/n^1.3\n");
    } else if (opcion_ini == 4) { //Creacion de monticulo
        printf("\n\t n\t\t\tt(n)\t      t(n)/n^0.8\t  t(n)/n^1.0\t      ");
        printf("t(n)/n^1.2\n");
    }


}

void tiempos (int opcion_ini, pmonticulo monticulo) {
    double t=0;
    int v[256000];
    int n;

    cabeceras(opcion_ini);

    for(n = 500; n<=256000 && t < 500000; n = 2*n) {
        inicializaciones(opcion_ini, v, n);
        t = calc_tmp(v,n, monticulo, opcion_ini);

        if (t<500){
            t = calc_tmp_medio(opcion_ini,v,n,monticulo);
            cotas_s(opcion_ini, t, n);

        } else {
            cotas(opcion_ini, t, n);
        }
    }
}

void imprTiemp_creacionMont (int opcion_ini, pmonticulo monticulo){
    double t=0;
    int v[256000];
    int n, i;

    cabeceras(opcion_ini);
    for(n = 500; n<=256000 && t < 500000; n = 2*n) {
        aleatorio(v,n);
        t = tiempo_monticulo(v,n, monticulo); //funcion que calcula el tiempo


        if (t<500){
            for (i = 0; i<1000; i++){
                t = t + tiempo_monticulo(v,n,monticulo);
            }
            t = t/1000;
            cotas_s(opcion_ini,t,n);

        } else {
            cotas(opcion_ini,t,n);
        }
    }
}

bool correcionDeMonticulo(pmonticulo M) {
    for (int i = 0; i <= M->ultimo; ++i) {
        int hijoIzquierdo = 2 * i + 1;
        int hijoDerecho = 2 * i + 2;
        if (hijoIzquierdo<=M->ultimo && M->vector[hijoIzquierdo]<M->vector[i]) {
            return false;
        }
        if (hijoDerecho <= M->ultimo && M->vector[hijoDerecho] < M->vector[i]) {
            return false;
        }
    }
    return true;
}


void pruebaMonticulo(pmonticulo M){
    int i, n=10;
    int v[TAM];


    printf("PRUEBAS DE MONTÍCULOS:\n\n");

    crearMonticulo(v,n,M,3);
    for(i = 0; i <= M->ultimo; i++){
        printf("%d ",M->vector[i]);
    }
    if(correcionDeMonticulo(M)){
        printf("\nManera aleatoria: Montículo correcto\n\n");
    }else{
        printf("\nManera aleatoria: Montículo incorrecto\n\n");
    }
    ;
    crearMonticulo(v,n,M,2);
    for(i = 0; i <= M->ultimo; i++){
        printf("%d ",M->vector[i]);
    }
    if(correcionDeMonticulo(M)){
        printf("\nManera descendente: Montículo correcto\n\n");
    }else{
        printf("\nManera aleatoria: Montículo incorrecto\n\n");
    }

    crearMonticulo(v,n,M,1);
    for(i = 0; i <= M->ultimo; i++){
        printf("%d ",M->vector[i]);
    }
    if(correcionDeMonticulo(M)){
        printf("\nManera ascendente: Montículo correcto\n\n");
    }
    else{
        printf("\nManera aleatoria: Montículo incorrecto\n\n");
    }
}


int main() {
    //int v[20];
    inicializar_semilla();

    pmonticulo monticulo = malloc(sizeof(struct monticulo));


    pruebaMonticulo(monticulo);

    printf("\nCreacion de montículos\n");
    imprTiemp_creacionMont(4, monticulo);
    printf("\nOrdenacion de montículos con vector aleatorio\n");
    tiempos(1,monticulo);
    printf("\nOrdenacion de montículos con vector ordenado descendentemente\n");
    tiempos(2,monticulo);
    printf("\nOrdenacion de monticulos con vector ordenado ascendentemente\n");
    tiempos(3,monticulo);


    printf("\nCreacion de montículos\n");
    imprTiemp_creacionMont(4, monticulo);
    printf("\nOrdenacion de montículos con vector aleatorio\n");
    tiempos(1,monticulo);
    printf("\nOrdenacion de montículos con vector ordenado descendentemente\n");
    tiempos(2,monticulo);
    printf("\nOrdenacion de montículos con vector ordenado ascendentemente\n");
    tiempos(3,monticulo);
    //ord_monticulo(v,20,monticulo);


    /*for (int i = 0; i <= 19; i++) {
        printf("%d ", v[i]);
    }*/

    free(monticulo);

    return 0;
}