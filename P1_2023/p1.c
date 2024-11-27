#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

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

int sumSubMax1 (int v [], int n) {
    int sumaMax = 0, estaSuma;
    int i, j;
    
    for (i = 0; i < n; i++) {
        estaSuma = 0;
        for (j = i; j < n; j++) {
            estaSuma = estaSuma + v[j];
            if (estaSuma > sumaMax) {
                sumaMax = estaSuma;
            }
        }
    }
    return sumaMax;
}


int sumSubMax2 (int v [], int n) {
    int estaSuma = 0, sumaMax = 0;
    int j;

    for (j = 0; j < n; j++) {
        estaSuma = estaSuma + v[j];
        if (estaSuma > sumaMax) {
            sumaMax = estaSuma;
        } else if (estaSuma < 0) {
            estaSuma = 0;
        }
    }
    return sumaMax;
}


void test1() {
    int j;
    int Matr [6] [5] = {{-9, 2, -5, -4, 6},
                        {4, 0, 9, 2, 5},
                        {-2,-1,-9, -7, -1},
                        {9, -2, 1, -7, -8},
                        {15, -2, -5, -4, 16},
                        {7, -5, 6, 7, -7}};

    printf("Secuencia\t\tResAS1\tResA2\n");
    for (j=0; j<6; j++){
        int fila[5];
        int i;
        for(i = 0; i < 5; i++) {
            fila[i] = Matr[j][i];
        }
    
        for(i = 0; i < 5; i++) {
            printf ("%3d", fila[i]);
        }
        printf ("\t\t%d\t%d\n", sumSubMax1(fila, 5), sumSubMax2(fila, 5));
    }
}


void test2() {
    int v[9];
    int i, j;
    printf("\t\t\t\t  sumaSubMax1\t sumaSubMax2\n");
    for (i=0; i<10; i++) {
        aleatorio(v, 9);
        printf("[");
        for(j = 0; j < 9; j++) {
            printf("%3d",v[j]);
        }
        printf (" ]%15d%15d\n", sumSubMax1(v,9), sumSubMax2(v,9));
    }
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

int elec (int op, int * vec, int n){
    if (op == 1) {
        return sumSubMax1(vec,n);
    }else if (op == 2) {
        return sumSubMax2(vec,n);
    }
        
}

//PROBAR A PASAR sumSubMax como parametro para reducir a 1 funcion solo
void tiempos(int opcion){
    double tiempo_inicio = 0, tiempo_final = 0, t = 0;
    int n, cont;
    int v[320000];
    char * eleccion;

    if (opcion == 1){
        eleccion = "\nSumaSubMax1\n\t n\t\t\tt(n)\t\tt(n)/n^1.8\t    t(n)/n^2\t        t(n)/n^2.2\n";
    } else if (opcion == 2){
        eleccion = "\nSumaSubMax2\n\t n\t\t\tt(n)\t\tt(n)/n^0.8\t    t(n)/n^1\t        t(n)/n^1.2\n";
    }
    printf("%s",eleccion);    
    
    for (n = 500; n <=256000 && t < 500000; n = 2*n) {
        aleatorio(v,n);
        tiempo_inicio = microsegundos();
        elec(opcion, v, n);
        tiempo_final = microsegundos();
        t = tiempo_final - tiempo_inicio;
        if(t<500){
            tiempo_inicio= microsegundos();
            for (cont=0; cont<1000; cont++){
                elec(opcion, v, n);
            }
            tiempo_final = microsegundos();
            t = (tiempo_final - tiempo_inicio)/1000;
            if (opcion == 1) {
                printf("(*)%7d\t%20.5f%20.6f%20.6f%20.6f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2))),(t/(pow(n, 2.2))));
            }else if (opcion == 2) {
                printf("(*)%7d\t%20.5f%20.6f%20.6f%20.6f\n",n,t,(t/(pow(n, 0.8))),(t/(pow(n, 1))),(t/(pow(n, 1.2))));
            }
        }else {
            if (opcion == 1) {
                printf("%10d\t%20.5f%20.6f%20.6f%20.6f\n",n,t,(t/(pow(n, 1.8))),(t/(pow(n, 2))),(t/(pow(n, 2.2))));
            }else if (opcion == 2) {
                printf("%10d\t%20.5f%20.6f%20.6f%20.6f\n",n,t,((double)t/(pow(n, 0.8))),((double)t/(pow(n, 1))),((double)t/(pow(n, 1.2))));
            }
        }
    }
}

int main () {
    inicializar_semilla();
    test1();
    printf("\n");
    test2();
    printf("\n");
    tiempos(1);
    tiempos(2);
    return 0;
}