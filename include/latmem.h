/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#ifndef _LATINO_MEM_H_
#define _LATINO_MEM_H_

#include "latino.h"

void *latM_asignar(lat_mv *mv, size_t size);
size_t latM_tamanio(void *ptr);
void *latM_reasignar(lat_mv *mv, void *ptr, size_t size);
void latM_liberar(lat_mv *mv, void *ptr);

#endif /* !_LATINO_MEM_H_ */
