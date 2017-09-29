/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#ifndef _LATINO_DIC_H_
#define _LATINO_DIC_H_

#include <stdlib.h>

#include "latlist.h"

typedef struct _lat_objeto lat_objeto;
typedef struct lat_mv lat_mv;

/**\brief Valor hash */
typedef struct hash_val {
    char llave[64];
    lat_objeto *valor;
} hash_val;

/**\brief Mapa de valores hash */
typedef struct hash_map {
    lista *buckets[256];
    size_t longitud;
} hash_map;

#define latH_longitud(hm) (hm)->longitud

int latH_hash(const char *key);

#endif /* !_LATINO_DIC_H_ */
