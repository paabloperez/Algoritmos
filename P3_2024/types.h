#ifndef UNTITLED_TYPES_H
#define UNTITLED_TYPES_H
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define NUM_CLAVES 19062
#include <stdio.h>
#include <stdlib.h>

typedef struct entrada_ {
    int ocupada;
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} entrada;

typedef unsigned int pos;
typedef entrada *tabla_cerrada;

typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
} item;

#endif //UNTITLED_TYPES_H
