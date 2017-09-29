/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#ifndef _LATINO_GC_H_
#define _LATINO_GC_H_

#include "latino.h"

/**\brief Bandera habilitar o deshabilitar GC */
#define HABILITAR_GC 0

void gc_agregar(lat_mv *mv, lat_objeto *o);
void gc_checar(lat_mv *mv);
void gc_recolectar(lat_mv *mv);

#endif /* _LATINO_GC_H_ */
