#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include "types.h"

pos dispersionA(char *clave, int tamTabla){
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 0; i <= n; ++i) {
        valor+=clave[i];
    }
    return valor % tamTabla;
}

pos dispersionB(char *clave, int tamTabla){
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 0; i <= n; ++i){
        valor = (valor<<5) + clave[i];
    }
    return valor % tamTabla;
}

int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

void inicializar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    for (i = 0; i < tam; ++i) {
        diccionario[i].ocupada = 0;
        diccionario[i].clave[0] = '\0';
        diccionario[i].sinonimos[0] = '\0';
    }
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam,
                   int *colisiones, pos (*dispersion)(char *, int),
                   pos (*resol_colisiones)(pos, unsigned int)) {
    pos pos_ini = dispersion(clave, tam);
    *colisiones = 0;
    int intento;
    for (intento = 0; intento < tam; intento++) {
        pos pos = (pos_ini + resol_colisiones(0, intento)) % tam;
        if (diccionario[pos].ocupada && strcmp(diccionario[pos].clave, clave) == 0) {
            return pos;
        }
        if (!diccionario[pos].ocupada) {
            break;
        }
        (*colisiones)++;
    }
    return -1;
}

int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario,
                     int tam, pos (*dispersion)(char *, int), pos (*resol_colisiones)(int, int)) {
    pos pos_ini = dispersion(clave, tam);
    int colisiones = 0;
    int intento;
    for (intento = 0; intento < tam; intento++) {
        pos pos = (pos_ini + resol_colisiones(0, intento)) % tam;
        if ((*diccionario)[pos].ocupada == 0) {
            strcpy((*diccionario)[pos].clave, clave);
            strcpy((*diccionario)[pos].sinonimos, sinonimos);
            (*diccionario)[pos].ocupada = 1;
            return colisiones;
        }
        colisiones++;
    }
    return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    printf("{\n");
    for (i = 0; i < tam; ++i) {
        if (diccionario[i].ocupada) {
            printf("%d - (%s)\n", i, diccionario[i].clave);
        } else {
            printf("%d -\n", i);
        }
    }
    printf("}\n");
}

pos ndispersion (char *clave, int tamTabla){
    if(strcmp(clave, "ANA") == 0) return 7;
    if(strcmp(clave, "JOSE") == 0) return 7;
    if(strcmp(clave, "OLGA") == 0) return 7;
    return 6;
}

pos resol_lineal(pos pos_ini, unsigned int num_intento) {
    return num_intento;
}

pos resol_cuadratica(pos pos_ini, unsigned int num_intento) {
    return num_intento * num_intento;
}

pos resol_doble(pos pos_ini, unsigned int num_intento) {
    return num_intento * (10007 - (pos_ini % 10007));
}

int insertar_pruebas(tabla_cerrada diccionario, int tamTabla,
                     pos (*dispersion)(char *, int),
                     pos (*resol_colisiones)(pos, unsigned int)) {
    char *palabras[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN"};
    char *sinonimos[] = {"sinonimos1", "sinonimos2", "sinonimos3", "sinonimos4", "sinonimos5", "sinonimos6"};
    int total_colisiones = 0;
    int num_palabras = sizeof(palabras) / sizeof(palabras[0]);
    int i;

    for (i = 0; i < num_palabras; ++i) {
        int colisiones = insertar_cerrada(palabras[i], sinonimos[i], &diccionario, tamTabla, dispersion, (pos (*) (int, int))resol_colisiones);
        if (colisiones >= 0) {
            total_colisiones += colisiones;
        } else {
            printf("Error al insertar %s\n", palabras[i]);
        }
    }
    return total_colisiones;
}

void buscar_pruebas(tabla_cerrada diccionario, int tamTabla,
                    pos (*dispersion)(char *, int),
                    pos (*resol_colisiones)(pos, unsigned int)) {
    char *palabras[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN", "CARLOS"};
    int num_palabras = sizeof(palabras) / sizeof(palabras[0]);
    int colisiones;
    int i;

    for (i = 0; i < num_palabras; i++) {
        colisiones = 0;
        pos posicion = buscar_cerrada(palabras[i], diccionario, tamTabla, &colisiones, dispersion, resol_colisiones);

        if (posicion != -1) {
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", palabras[i], diccionario[posicion].clave, colisiones);
        } else {
            printf("No encuentro: %s, colisiones: %d\n", palabras[i], colisiones);
        }
    }
}

void test(tabla_cerrada diccionario, int tam) {
    int total_colisiones_lineal = 0;
    int total_colisiones_cuadratica = 0;

    printf("***TABLA CERRADA LINEAL\n");
    total_colisiones_lineal = insertar_pruebas(diccionario, tam, ndispersion, resol_lineal);
    mostrar_cerrada(diccionario, tam);
    printf("Numero total de colisiones al insertar los elementos: %d\n", total_colisiones_lineal);
    buscar_pruebas(diccionario, tam, ndispersion, resol_lineal);

    inicializar_cerrada(diccionario, tam);

    printf("\n***TABLA CERRADA CUADRATICA\n");
    total_colisiones_cuadratica = insertar_pruebas(diccionario, tam, ndispersion, resol_cuadratica);
    mostrar_cerrada(diccionario, tam);
    printf("Numero total de colisiones al insertar los elementos: %d\n", total_colisiones_cuadratica);
    buscar_pruebas(diccionario, tam, ndispersion, resol_cuadratica);

    printf("\n***TABLA CERRADA DOBLE\n");
    total_colisiones_cuadratica = insertar_pruebas(diccionario, tam, ndispersion, resol_doble);
    mostrar_cerrada(diccionario, tam);
    printf("Numero total de colisiones al insertar los elementos: %d\n", total_colisiones_cuadratica);
    buscar_pruebas(diccionario, tam, ndispersion, resol_cuadratica);
}

int inserciones_sinonimos(tabla_cerrada diccionario, int tam,
                          pos (*resol_colisiones)(pos, unsigned int),
                          pos (*dispersion)(char *, int)) {
    item *datos = malloc(38197 * sizeof(item));
    int num_sinonimos = leer_sinonimos(datos);

    if (datos == NULL) {
        printf("Error al reservar memoria para datos.\n");
    }

    if (num_sinonimos < 0) {
        printf("Error al leer los sinónimos.\n");
        free(datos);
        return -1;
    }

    int total_colisiones = 0;
    for (int i = 0; i < num_sinonimos; i++) {
        int colisiones = insertar_cerrada(datos[i].clave, datos[i].sinonimos, &diccionario, tam, dispersion, (pos (*)(int, int)) resol_colisiones);
        if (colisiones >= 0) {
            total_colisiones += colisiones;
        } else {
            printf("Error al insertar %s\n", datos[i].clave);
        }
    }
    free(datos);
    return total_colisiones;
}

void test_insercion_sinonimos(tabla_cerrada diccionario, int tam) {
    int total_colisiones = 0;

    inicializar_cerrada(diccionario, tam);
    printf("\nInsertando con exploracion lineal y dispersionA:\n");
    total_colisiones = inserciones_sinonimos(diccionario, tam, resol_lineal, dispersionA);
    printf("Numero total de colisiones al insertar con función de dispersión y resolución de colisiones seleccionada: %d\n", total_colisiones);

    inicializar_cerrada(diccionario, tam);
    printf("\nInsertando con exploracion lineal y dispersionB:\n");
    total_colisiones = inserciones_sinonimos(diccionario, tam, resol_lineal, dispersionB);
    printf("Numero total de colisiones al insertar con función de dispersión y resolución de colisiones seleccionada: %d\n", total_colisiones);

    inicializar_cerrada(diccionario, tam);
    printf("\nInsertando con exploracion cuadratica y dispersionA:\n");
    total_colisiones = inserciones_sinonimos(diccionario, tam, resol_cuadratica, dispersionA);
    printf("Numero total de colisiones al insertar con función de dispersión y resolución de colisiones seleccionada: %d\n", total_colisiones);

    inicializar_cerrada(diccionario, tam);
    printf("\nInsertando con exploracion cuadratica y dispersionB:\n");
    total_colisiones = inserciones_sinonimos(diccionario, tam, resol_cuadratica, dispersionB);
    printf("Numero total de colisiones al insertar con función de dispersión y resolución de colisiones seleccionada: %d\n", total_colisiones);

    inicializar_cerrada(diccionario, tam);
    printf("\nInsertando con exploracion doble y dispersionA:\n");
    total_colisiones = inserciones_sinonimos(diccionario, tam, resol_doble, dispersionA);
    printf("Numero total de colisiones al insertar con función de dispersión y resolución de colisiones seleccionada: %d\n", total_colisiones);

    inicializar_cerrada(diccionario, tam);
    printf("\nInsertando con exploracion doble y dispersionB:\n");
    total_colisiones = inserciones_sinonimos(diccionario, tam, resol_doble, dispersionB);
    printf("Numero total de colisiones al insertar con función de dispersión y resolución de colisiones seleccionada: %d\n", total_colisiones);

    printf("\n");
}

double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0){
        return 0.0;
    }
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void tabla (int expl, int disp) {
    printf("Dispersion cerrada ");
    if (expl == 1) {
        printf("lineal ");
        if (disp == 1) {
            printf("con dispersion A");
        }else {
            printf("con dispersion B");
        }
    }else if (expl == 2){
        printf("cuadratica ");
        if (disp == 1) {
            printf("con dispersion A");
        }else {
            printf("con dispersion B");
        }
    }else{
        printf("doble ");
        if (disp == 1) {
            printf("con dispersion A");
        }else {
            printf("con dispersion B");
        }
    }
    printf("\n");
}

void cabeceras(int disp){
    if (disp == 1){
    	printf("\tn\t\t\tt(n)\t     t(n)/n^0.95          t(n)/n^1           t(n)/n^1.1\n");
    }else{
    	printf("\tn\t\t\tt(n)\t     t(n)/n^0.98          t(n)/n^1           t(n)/n^1.02\n");
    }
}

void cotas (int expl, int disp, double t, int n) {
    if (expl == 1) {
        if (disp == 1) {
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.95))), (t/(pow(n, 1.0))),       (t/(pow(n, 1.1))));
        } else {
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.98))),(t/(pow(n, 1.0))),(t/(pow(n, 1.02))));
        }
    }else if (expl == 2) {
        if (disp == 1) {
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.95))), (t/(pow(n, 1.0))),       (t/(pow(n, 1.1))));
        } else {
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.98))),(t/(pow(n, 1.0))),(t/(pow(n, 1.02))));
        }
    }else{
        if (disp == 1) {
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.95))), (t/(pow(n, 1.0))),       (t/(pow(n, 1.1))));
        } else {
            printf("%10d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.98))),(t/(pow(n, 1.0))),(t/(pow(n, 1.02))));
        }
    }
}

void cotas_s (int expl, int disp, double t, int n) {
    if (expl == 1) {
        if (disp == 1) {
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.95))),     (t/(pow(n, 1.0 ))),    (t/(pow(n, 1.1))));
        } else{
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.98))),(t/(pow(n, 1.0))),(t/(pow(n, 1.02))));
        }
    }else if (expl == 2) {
        if (disp == 1) {
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.95))),     (t/(pow(n, 1.0 ))),    (t/(pow(n, 1.1))));
        } else{
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.98))),(t/(pow(n, 1.0))),(t/(pow(n, 1.02))));
        }
    }else{
        if (disp == 1) {
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.95))),     (t/(pow(n, 1.0 ))),    (t/(pow(n, 1.1))));
        } else{
            printf("(*)%7d\t%20.5f%20.9f%20.9f%20.9f\n",n,t,(t/(pow(n, 0.98))),(t/(pow(n, 1.0))),(t/(pow(n, 1.02))));
        }
    }
}

void indicar_cota(int expl, int disp){
    if(expl == 1){
        if(disp == 1){
            printf("\t\t\tCota subestimada: n ^ 0.95\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.1\n");
        }else{
            printf("\t\t\tCota subestimada: n ^ 0.98\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.02\n");
        }
    }else if(expl == 2){
        if(disp == 1){
            printf("\t\t\tCota subestimada: n ^ 0.95\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.1\n");
        }else{
            printf("\t\t\tCota subestimada: n ^ 0.98\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.02\n");
        }
    }else{
        if(disp == 1){
            printf("\t\t\tCota subestimada: n ^ 0.95\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.1\n");
        }else{
            printf("\t\t\tCota subestimada: n ^ 0.98\tCota ajustada: n ^ 1\tCota sobrestimada: n ^ 1.02\n");
        }
    }
}

pos (*elec_expl(int expl)) (pos, unsigned int){
    if (expl == 1) {
        return resol_lineal;
    }else if(expl == 2){
        return resol_cuadratica;
    }else{
        return resol_doble;
    }
}

pos (*elec_disp(int disp)) (char *clave, int numTabla){
    if (disp == 1) {
        return dispersionA;
    }else {
        return dispersionB;
    }
}

int cargar_claves(const char *nombre, char claves[NUM_CLAVES][LONGITUD_CLAVE]){
    int i = 0;
    FILE *archivo = fopen(nombre, "r");

    if (!archivo){
        perror("Error al abrir el archivo\n");
        return -1;
    }

    while (i < NUM_CLAVES && fscanf(archivo, "%s", claves[i]) == 1){
        i++;
    }
    fclose(archivo);
    return i;
}

void tiempo (int expl, int disp, int tam, tabla_cerrada diccionario) {
    int i, j, k, indice_aleatorio;
    int colisiones = 0;
    double t1, t2, tf;
    char *clave;
    char claves[NUM_CLAVES][LONGITUD_CLAVE];
    int a[8] = {125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    int total_colisiones = inserciones_sinonimos(diccionario, tam, elec_expl(expl), elec_disp(disp));
    printf("Insertando 19062 elementos... Numero total de colisiones: %d\n", total_colisiones);
    cabeceras(disp);
    int num_claves = cargar_claves("sinonimos.txt", claves);

    if (num_claves < 1){
        printf("Error al cargar las claves.\n");
        return;
    }

    for(i = 0; i <= 7; i++){
        colisiones = 0;
        t1 = microsegundos();
        for(j = 0; j < a[i]; j++){
            indice_aleatorio = rand() % num_claves;
            clave = claves[indice_aleatorio];
            buscar_cerrada(clave, diccionario, tam, &colisiones, elec_disp(disp), elec_expl(expl));
        }
        t2 = microsegundos();
        tf = t2 - t1;

        if (tf < 500){
            t1 = microsegundos();
            for (k = 0; k < 1000; k++){
                for (j = 0; j < a[i]; j++){
                    indice_aleatorio = rand() % num_claves;
                    clave = claves[indice_aleatorio];
                    buscar_cerrada(clave, diccionario, tam, &colisiones, elec_disp(disp), elec_expl(expl));
                }
            }
            t2 = microsegundos();
            tf = (t2 - t1) / 1000;
            cotas_s(expl, disp, tf, a[i]);
        }else{
            cotas(expl, disp, tf, a[i]);
        }
    }
    indicar_cota(expl, disp);
}

void tiempos (tabla_cerrada diccionario, int tam) {
    int expl, disp;
    for (expl = 1; expl <= 3; expl++) {
        for (disp = 1; disp <= 2; disp++) {
            inicializar_cerrada(diccionario, tam);
            tabla(expl, disp);
            tiempo(expl, disp, tam, diccionario);
            printf("\n");
        }
    }
}

int main(void) {
    int tam = 38197;
    tabla_cerrada diccionario = malloc(tam * sizeof(entrada));

    if (diccionario != NULL) {
        inicializar_cerrada(diccionario, tam);
        test(diccionario, 11);
        test_insercion_sinonimos(diccionario, tam);
        tiempos(diccionario, tam);
    } else {
        printf("Error al asignar memoria para el diccionario.\n");
        return -1;
    }

    free(diccionario);
    return 0;
}
