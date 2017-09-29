/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#include "latino.h"

#define LIB_DIC_NAME "dic"

static void dic_longitud(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    lat_objeto *tmp = NULL;
    tmp =
        latC_crear_numerico(mv, (double)latH_longitud(latC_checar_dic(mv, o)));
    latC_apilar(mv, tmp);
}

static void dic_llaves(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    hash_map *m = latC_checar_dic(mv, o);
    lista *lst = latL_crear(mv);
    int i;
    for (i = 0; i < 256; i++) {
        lista *list = m->buckets[i];
        if (list != NULL) {
            if (list->longitud > 0) {
                LIST_FOREACH(list, primero, siguiente, cur) {
                    if (cur->valor != NULL) {
                        char *str_key = ((hash_val *)cur->valor)->llave;
                        latL_agregar(mv, lst, latC_crear_cadena(mv, str_key));
                    }
                }
            }
        }
    }
    lat_objeto *tmp = latC_crear_lista(mv, lst);
    latC_apilar(mv, tmp);
}

static void dic_valores(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    hash_map *m = latC_checar_dic(mv, o);
    lista *lst = latL_crear(mv);
    int i;
    for (i = 0; i < 256; i++) {
        lista *list = m->buckets[i];
        if (list != NULL) {
            if (list->longitud > 0) {
                LIST_FOREACH(list, primero, siguiente, cur) {
                    if (cur->valor != NULL) {
                        lat_objeto *val =
                            (lat_objeto *)((hash_val *)cur->valor)->valor;
                        latL_agregar(mv, lst, val);
                    }
                }
            }
        }
    }
    lat_objeto *tmp = latC_crear_lista(mv, lst);
    latC_apilar(mv, tmp);
}

static const lat_CReg libdic[] = {{"longitud", dic_longitud, 1},
                                  {"llaves", dic_llaves, 1},
                                  {"valores", dic_valores, 1},
                                  {"vals", dic_valores, 1},
                                  {NULL, NULL, 0}};

void latC_abrir_liblatino_diclib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_DIC_NAME, libdic);
}
