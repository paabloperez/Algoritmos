#include <stdio.h>
#include <sys/time.h>
#include <math.h>

int fib1(int n){
    if(n<2){
        return n;
    }else{
        return fib1(n-1) + fib1(n-2);
    }
}

int fib2(int n){
    int i, j, k;
    i=1;    j=0;
    for (k = 1; k <= n; ++k) {
        j=i+j;
        i=j-i;
    }
    return j;
}

int fib3(int n){
    int i, j, k, h, t;
    i = 1;  j=0;    k=0;    h=1;    t=0;
    while(n>0){
        if((n%2) != 0){
            t= j*h;
            j= (i*h) + (j*k) + t;
            i = (i*k) + t;
        }
        t = h*h;
        h = (2*k*h) +t;
        k = (k*k) + t;
        n = n/2;
    }
    return j;
}


void printVector(int n, int v[n]){
    int i;
    for (i = 0; i < n; ++i) {
        printf("%d\t", v[i]);
    }
    printf("\n");
}

void test(){
    int i, j, k;
    int res[20];

    //TEST fib1
    printf("Test Fib1:\n");
    for (i = 1; i <= 20 ; ++i) {
        res[i-1] = fib1(i);
    }
    printVector(20, res);
    printf("\n");
    //TEST fib2
    printf("Test Fib2:\n");
    for (j = 1; j <= 20 ; ++j) {
        res[j-1] = fib2(j);
    }
    printVector(20, res);
    printf("\n");
    //TEST fib3
    printf("Test Fib3:\n");
    for (k = 1; k <= 20 ; ++k) {
        res[k-1] = fib3(k);
    }
    printVector(20, res);
    printf("\n");
}


double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0){
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void cabeceras(){
    printf("\t   n\t\t t\t Cota subestimada\t Cota ajustada\t Cota sobreestimada\n");
}

void tiempos_fib1(){
    double tinicio, tfinal, x, y, z, ta, tb, t1, t2, t[5];
    int v[5]={2, 4, 8, 16, 32};
    int i, j, k;

    printf("Tiempos y complejidades computacionales de FIB1:\n");
    cabeceras();
    for (i = 0; i < 5; ++i) {
        tinicio = microsegundos();
        fib1(v[i]);
        tfinal = microsegundos();
        t[i] = tfinal - tinicio;
        if (t[i] < 500){
            ta = microsegundos();
            for (j = 0; j < 500; ++j) {
                int v1[5]={2, 4, 8, 16, 32};
                fib1(v1[i]);
            }
            tb = microsegundos();
            t1 = tb - ta;   //tiempo de inicializar + funcion
            ta = microsegundos();
            for (k = 0; k < 500; ++k) {
                int v1[5]={2, 4, 8, 16, 32};
            }
            tb = microsegundos();
            t2 = tb - ta;   //tiempo de inicializar solo
            t[i] = (t1-t2)/500; //tiempo de todo - tiempo de inicializar entre 500
            x = t[i] / pow(1.1, v[i]);
            y = t[i] / pow(1.618, v[i]);
            z = t[i] / pow(2, v[i]);
            printf("%12d %15.4f* %15.6f \t    %15.6f %15.8f \n", v[i], t[i], x, y, z);
        }else{
            x = t[i] / pow(1.1, v[i]);
            y = t[i] / pow(1.618, v[i]);
            z = t[i] / pow(2, v[i]);
            printf("%12d %15.4f %15.6f \t    %15.6f %15.8f \n", v[i], t[i], x, y, z);
        }
    }
    printf("Cota subestimada: t(n) / 1.1^n\tCota ajustada: t(n) / 1.618^n\tCota sobreestimada: t(n) / 2^n\n");
}


void tiempos_fib2(){
    double tinicio, tfinal, x, y, z, ta, tb, t1, t2, t[5];
    int v[5]={1000, 10000, 100000, 1000000, 10000000};
    int i, j, k;

    printf("Tiempos y complejidades computacionales de FIB2:\n");
    cabeceras();
    for (i = 0; i < 5; ++i) {
        tinicio = microsegundos();
        fib2(v[i]);
        tfinal = microsegundos();
        t[i] = tfinal - tinicio;
        if (t[i] < 500){
            ta = microsegundos();
            for (j = 0; j < 500; ++j) {
                int v2[5]={1000, 10000, 100000, 1000000, 10000000};
                fib2(v2[i]);
            }
            tb = microsegundos();
            t1 = tb - ta;
            ta = microsegundos();
            for (k = 0; k < 500; ++k) {
                int v2[5]={1000, 10000, 100000, 1000000, 10000000};
            }
            tb = microsegundos();
            t2 = tb - ta;
            t[i] = (t1-t2)/500;
            x = t[i] / pow(v[i], 0.8);
            y = t[i] / v[i];
            z = t[i] / v[i] * log(v[i]);
            printf("%12d %15.4f* %15.6f \t    %15.6f %15.8f \n", v[i], t[i], x, y, z);
        }else{
            x = t[i] / pow(v[i], 0.8);
            y = t[i] / v[i];
            z = t[i] / v[i] * log(v[i]);
            printf("%12d %15.4f %15.6f \t    %15.6f %15.8f \n", v[i], t[i], x, y, z);
        }
    }
    printf("Cota subestimada: t(n) / n^0.8\tCota ajustada: t(n) / n\tCota sobreestimada: t(n) / n * log(n)\n");
}


void tiempos_fib3(){
    double tinicio, tfinal, x, y, z, ta, tb, t1, t2, t[5];
    int v[5]={1000, 10000, 100000, 1000000, 10000000};
    int i, j, k;

    printf("Tiempos y complejidades computacionales de FIB3:\n");
    cabeceras();
    for (i = 0; i < 5; ++i) {
        tinicio = microsegundos();
        fib3(v[i]);
        tfinal = microsegundos();
        t[i] = tfinal - tinicio;
        if (t[i] < 500){
            ta = microsegundos();
            for (j = 0; j < 500; ++j) {
                int v2[5]={1000, 10000, 100000, 1000000, 10000000};
                fib3(v2[i]);
            }
            tb = microsegundos();
            t1 = tb - ta;
            ta = microsegundos();
            for (k = 0; k < 500; ++k) {
                int v3[5]={1000, 10000, 100000, 1000000, 10000000};
            }
            tb = microsegundos();
            t2 = tb - ta;
            t[i] = (t1-t2)/500;
            x = t[i] / sqrt(log(v[i]));
            y = t[i] / log(v[i]);
            z = t[i] / pow(v[i], 0.5);
            printf("%12d %15.4f* %15.6f \t    %15.6f %15.8f \n", v[i], t[i], x, y, z);
        }else{
            x = t[i] / sqrt(log(v[i]));
            y = t[i] / log(v[i]);
            z = t[i] / pow(v[i], 0.5);
            printf("%12d %15.4f %15.6f \t    %15.6f %15.8f \n", v[i], t[i], x, y, z);
        }
    }
    printf("Cota subestimada: t(n) / sqrt(log(n))\tCota ajustada: t(n)/ log(n)\tCota sobreestimada: t(n) / n^0.5\n");
}

int main() {
    //TEST
    test();
    printf("\n");

    //TIEMPOS
    tiempos_fib1();
    printf("\n");
    tiempos_fib2();
    printf("\n");
    tiempos_fib3();
    printf("\n");

    return 0;
}
