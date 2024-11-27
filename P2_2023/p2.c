#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
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
void ord_ins(int v[],int n){
    int i;
    int x;
    int j;
    for(i = 1; i < n; i++){
        x = v[i];
        j = i-1;
        while (j >= 0 && v[j] > x){
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1] = x;
    }

}


void ord_shell (int v [], int n) {
    int incremento, i, j, tmp;
    bool seguir;

    incremento = n;
    do {
        incremento = incremento/2;
        for (i = incremento; i < n; i++){
            tmp = v[i];
            j = i;
            seguir = true;
            while ((j-incremento >= 0) && seguir){
                if (tmp < v[j-incremento]){
                    v[j]= v[j-incremento];
                    j = j-incremento;
                }else{
                    seguir=false;
                }
            }
            v[j] = tmp;
        }
    } while (incremento != 1);
}


int check_ord (int v[], int n){
    int i;
    for (i=1; i<n; i ++) {
        if (v[i]< v[i-1]){
            return 0;
        }
    }
    return 1;
}

void ordenaciones(int v[], int t[], int n){
    int j;

    printf("\nOrdenacion por insercion\n");
    for(j = 0; j < n; j++) {
        printf("%3d",v[j]);
    }
    printf("\nOrdenado? %d\n", check_ord(v,n));

    ord_ins(v,n);
    for(j = 0; j < n; j++) {
        printf("%3d",v[j]);
    }
    printf("\nOrdenado? %d\n", check_ord(v,n));

    printf("\nOrdenacion shell\n");
    for(j = 0; j < n; j++) {
        printf("%3d",t[j]);
    }
    printf("\nOrdenado? %d\n", check_ord(t,n));
    ord_shell(t,n);
    for(j = 0; j < n; j++) {
        printf("%3d",t[j]);
    }
    printf("\nOrdenado? %d\n", check_ord(t,n));
}


void inicializaciones_aleatorias() {
    int v[17], t[17];
    int j;
    printf("INICIALIZACION ALEATORIA\n");
    aleatorio(v,17);

    for(j = 0; j < 17; j++) {
        t[j] = v[j]; //creamos copia del vector aleatorio
    }

    ordenaciones(v, t, 17);

}

void inicializacion_Desc() {
    int v[10], t[10];

    printf("\nINICIALIZACION DESCENDENTE\n");

    descendente(v,10);
    descendente(t,10);

    ordenaciones(v,t,10);
}

void inicializacion_Asc() {
    int v[10], t[10];

    printf("\nINICIALIZACION ASCENDENTE\n");

    ascendente(v,10);
    ascendente(t,10);

    ordenaciones(v,t,10);
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

double calc_tmp(int opcion_ord, int v[], int n){
    double tiempo_inicio=0, tiempo_final=0, t=0;
    if (opcion_ord == 1) {
        tiempo_inicio = microsegundos();
        ord_ins(v,n);
        tiempo_final = microsegundos();
    }else if (opcion_ord == 2) {
        tiempo_inicio = microsegundos();
        ord_shell(v,n);
        tiempo_final = microsegundos();
    }

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

double calc_tmp_medio(int opcion_ord, int opcion_ini, int v[], int n){
    double tiempo_inicio=0, tiempo_final=0, t1=0, t2=0, tfin=0;
    int cont;
    if (opcion_ord == 1) {
        tiempo_inicio = microsegundos();
        for(cont=0; cont<1000; cont++) {
            inicializaciones(opcion_ini, v, n);
            ord_ins(v,n);
        }
        tiempo_final = microsegundos();
    }else if (opcion_ord == 2) {
        tiempo_inicio = microsegundos();
        for(cont=0; cont<1000; cont++) {
            ord_shell(v,n);
            inicializaciones(opcion_ini, v, n);
        }
        tiempo_final = microsegundos();
    }

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

void cotas (int opcion_ini, int opcion_ord, double t, int n) {
    if (opcion_ord == 1) {
        if (opcion_ini == 1) { //insercion aleatorio
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.1))));
        } else if (opcion_ini == 2){ //insercion descendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.2))));
        } else if (opcion_ini == 3){ //insercion ascendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.8))),(t/(pow(n, 1.0))),(t/(pow(n, 1.2))));
        }
    }else if (opcion_ord == 2) {
        if (opcion_ini == 1) { //shell aleatorio
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 1.2))),(t/(pow(n, 1.4))));
        } else if (opcion_ini == 2){ //shell descendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 1.14))),(t/(pow(n, 1.3))));
        } else if (opcion_ini == 3){ //shell ascendente
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 1.14))),(t/(pow(n, 1.3))));
        }
    }
}

void cotas_s (int opcion_ini, int opcion_ord, double t, int n) {
    if (opcion_ord == 1) {
        if (opcion_ini == 1) { //insercion aleatorio
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.2))));
        } else if (opcion_ini == 2){ //insercion descendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2.0))),(t/(pow(n, 2.2))));
        } else if (opcion_ini == 3){ //insercion ascendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.8))),(t/(pow(n, 1.0))),(t/(pow(n, 1.2))));
        }
    }else if (opcion_ord == 2) {
        if (opcion_ini == 1) {//shell aleatorio
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 1.2))),(t/(pow(n, 1.4))));
        } else if (opcion_ini == 2){ //shell descendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 1.14))),(t/(pow(n, 1.3))));
        } else if (opcion_ini == 3){ //shell ascendente
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 1.0))),(t/(pow(n, 1.14))),(t/(pow(n, 1.3))));
        }
    }
}

void cabeceras(int opcion_ini, int opcion_ord) {
    char * eleccion;
    if (opcion_ord == 1){
        if (opcion_ini == 1) { //insercion aleatorio
            eleccion = "\n\t n\t\t\tt(n)\t      t(n)/n^1.8\t    t(n)/n^2\t      t(n)/n^2.2\n";
        } else if (opcion_ini == 2) { //insercion descendente
            eleccion = "\n\t n\t\t\tt(n)\t      t(n)/n^1.8\t    t(n)/n^2\t      t(n)/n^2.2\n";
        } else if (opcion_ini == 3) { //insercion ascendente
            eleccion = "\n\t n\t\t\tt(n)\t      t(n)/n^0.8\t  t(n)/n^1.0\t      t(n)/n^1.2\n";
        }
    } else if (opcion_ord == 2) {
        if (opcion_ini == 1) { //shell aleatorio
            eleccion = "\n\t n\t\t\tt(n)\t      t(n)/n^1.0\t  t(n)/n^1.2\t      t(n)/n^1.4\n";
        } else if (opcion_ini == 2) { //shell descendente
            eleccion = "\n\t n\t\t\tt(n)\t      t(n)/n^1.0\t t(n)/n^1.14\t      t(n)/n^1.3\n";
        } else if (opcion_ini == 3) { //shell ascendente
            eleccion = "\n\t n\t\t\tt(n)\t      t(n)/n^1.0\t t(n)/n^1.14\t      t(n)/n^1.3\n";
        }
    }
    printf("%s",eleccion);
}

void tiempos (int opcion_ini, int opcion_ord) {
    double t=0;
    int v[320000];
    int n;

    //opcion_ord = 1 ----> insercion
    //opcion_ord = 2 ----> shell

    cabeceras(opcion_ini, opcion_ord);

    for(n = 500; n<=256000 && t < 500000; n = 2*n) {
        inicializaciones(opcion_ini, v, n);
        t = calc_tmp(opcion_ord,v,n); //funcion que calcula el tiempo


        if (t<500){
            t = calc_tmp_medio(opcion_ord, opcion_ini, v, n);
            cotas_s(opcion_ini, opcion_ord, t, n);

        } else {
            cotas(opcion_ini, opcion_ord, t, n);

        }
    }
}


int main(){
    inicializar_semilla();
    inicializaciones_aleatorias();
    inicializacion_Desc();
    inicializacion_Asc();
    printf("\nOrdenacion por insercion con inicializacion aleatoria\n");
    tiempos(1,1);
    printf("\nOrdenacion por shell con inicializacion aleatoria\n");
    tiempos(1,2);
    printf("\nOrdenacion por insercion con inicializacion descendente\n");
    tiempos(2,1);
    printf("\nOrdenacion por shell con inicializacion descendente\n");
    tiempos(2,2);
    printf("\nOrdenacion por insercion con inicializacion ascendente\n");
    tiempos(3,1);
    printf("\nOrdenacion por shell con inicializacion ascendente\n");
    tiempos(3,2);
    printf("\n");
    return 0;
}
