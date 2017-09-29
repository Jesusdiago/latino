/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#define LATINO_CORE

#include "latdo.h"
#include "latast.h"
#include "latino.h"
#include "latmem.h"
#include "latmv.h"
#include "latobj.h"

struct lat_longjmp {
    struct lat_longjmp *previo;
    lat_jmpbuf b;
    volatile int status;
};

void latD_lanzar(lat_mv *mv, int errcode) {
    if (mv->error) {
        mv->error->status = errcode;
        LAT_THROW(mv, mv->error);
    } else {
        mv->status = errcode;
        mv->error = NULL;
        exit(EXIT_FAILURE);
    }
}

void str_concatenar(lat_mv *mv);
LATINO_API void latC_error(lat_mv *mv, const char *fmt, ...) {
    char buffer[MAX_INPUT_SIZE];
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);
    char *info = malloc(MAX_INPUT_SIZE);
    snprintf(info, MAX_INPUT_SIZE, "\033[1;31m%s:%d:%d:\033[0m ",
             mv->nombre_archivo, mv->nlin, mv->ncol);
    latC_apilar(mv, latC_crear_cadena(mv, info));
    latC_apilar(mv, latC_crear_cadena(mv, buffer));
    str_concatenar(mv);
    lat_objeto *err = latC_desapilar(mv);
    fprintf(stderr, "%s\n", latC_astring(mv, err));
    latD_lanzar(mv, LAT_ERRRUN);
}

LATINO_API int latC_llamar_funcion(lat_mv *mv, lat_objeto *func) {
    // printf("%s\n", func->nombre);
    struct lat_longjmp lj;
    lj.status = 0;
    lj.previo = mv->error;
    mv->error = &lj;
    LAT_TRY(mv, &lj, latMV_funcion_correr(mv, func););
    mv->error = lj.previo;
    return lj.status;
}
