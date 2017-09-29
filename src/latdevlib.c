/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#include "latino.h"

#define LIB_LAT_NAME "lat"

static void dev_mostrar(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    char *tmp = latC_astring(mv, o);
    printf("Tipo : %s\n", tipo(o->tipo));
    printf("Marca : %i\n", o->marca);
    printf("Num. Referencias : %i\n", o->nref);
    printf("Tamanio : %zu\n", o->tam);
    printf("Direccion : %p\n", o);
    printf("Contenido : %s\n", tmp);
    printf("------------------------------------\n");
    free(tmp);
}

void imprimir_pila(lat_mv *mv);
static void dev_imprimir_pila(lat_mv *mv) { imprimir_pila(mv); }

static const lat_CReg liblat[] = {{"mostrar", dev_mostrar, 1},
                                  {"imprimir_pila", dev_imprimir_pila, 0},
                                  {NULL, NULL, 0}};

void latC_abrir_liblatino_devlib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_LAT_NAME, liblat);
}
