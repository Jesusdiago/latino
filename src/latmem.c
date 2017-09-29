/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#include <stdlib.h>

#include "latdo.h"
#include "latino.h"
#include "latmem.h"
#include "latmv.h"

void *latM_asignar(lat_mv *mv, size_t size) {
    void *ptr = calloc(1, size);
    // void *ptr = malloc(size);
    if (ptr == NULL) {
        latC_error(mv, "Memoria insuficiente en el sistema");
    }
    if (mv) {
        // size_t tam = latM_tamanio(ptr);
        // printf("+++ memoria asignada: %zu, %p\n", tam, ptr);
        mv->memoria_usada += latM_tamanio(ptr);
    }
    return ptr;
}

size_t latM_tamanio(void *ptr) { return malloc_size(ptr); }

void *latM_reasignar(lat_mv *mv, void *ptr, size_t size) {
    // size_t mem_ini = latM_tamanio(ptr);
    void *value = realloc(ptr, size);
    if (value == NULL)
        latC_error(mv, "Memoria insuficiente en el sistema");
    /*printf("memoria inicial: %zu memoria nueva: %zu, %p\n",
    mem_ini, latM_tamanio(value), &value);*/
    return value;
}

void latM_liberar(lat_mv *mv, void *ptr) {
    if (ptr != NULL) //&& latM_tamanio(ptr) > 0
    {
        // printf("--- memoria liberada: %zu, %p\n",
        // latM_tamanio(ptr), ptr);
        if (mv != NULL) {
            // size_t tam = latM_tamanio(ptr);
            // printf("--- memoria liberada: %zu, %p\n", tam, ptr);
            mv->memoria_usada -= latM_tamanio(ptr);
        }
        ptr = NULL;
        free(ptr);
    }
}
