/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#ifndef _LATINO_H_
#define _LATINO_H_

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "latast.h"
#include "latcompat.h"
#include "latdic.h"
#include "latlist.h"
#include "latmv.h"
#include "latobj.h"

/** Version mayor de Latino */
#define LAT_VERSION_MAYOR "1"
/** Version menor de Latino */
#define LAT_VERSION_MENOR "1"
/** Version de correcion de errores */
#define LAT_VERSION_PARCHE "0"
/** Version de Latino */
#define LAT_VERSION                                                            \
    "Latino " LAT_VERSION_MAYOR "." LAT_VERSION_MENOR "." LAT_VERSION_PARCHE
/** Derechos de Latino */
#define LAT_DERECHOS                                                           \
    LAT_VERSION                                                                \
    "\nTodos los derechos reservados (C) 2015-2017. Latinoamerica"

#define LAT_ERRMEM 1
#define LAT_ERRRUN 2

/** Define el manejo de excepciones en Latino */
#define LAT_THROW(mv, j) longjmp((j)->b, 1)
#define LAT_TRY(mv, j, f)                                                      \
    if (_setjmp((j)->b) == 0) {                                                \
        f                                                                      \
    }
#define lat_jmpbuf jmp_buf

/** Indica si se desea debuguear el parser de bison */
extern int debug;

/** Indica que el parser no debe de devolver errores, se usa para REPL */
extern int parse_silent;

/* generado en:
http://www.patorjk.com/software/taag/#p=display&f=Graffiti&t=latino
Dibuja el logo
 */
#define LAT_LOGO                                                               \
    "\n.__          __  .__               \n|  | _____ _/  |_|__| ____   "     \
    "____  "                                                                   \
    "\n|  | \\__  \\\\   __\\  |/    \\ /  _ \\ \n|  |__/ __ \\|  | |  | "     \
    "  |  "                                                                    \
    "(  <_> )\n|____(____  /__| |__|___|  /\\____/ \n          \\/       "     \
    "      "                                                                   \
    "\\/        \n"

#define HISTORY_FILE ".lat_historial"

/** Afirmar (asset), sirve para testear una condicion */
#define latC_afirmar(cond) ((void)(false && (cond)))

/** Maximo numero de size_t */
#ifdef SIZE_MAX
#define LAT_SIZE_MAX SIZE_MAX
#else
#define LAT_SIZE_MAX ((size_t)-1)
#endif

/** Tamanio maximo de instrucciones bytecode de una fun */
#define MAX_BYTECODE_FUNCTION (1024 * 128)
/** Tamanio maximo de memoria virtual permitida*/
#define MAX_VIRTUAL_MEMORY (MAX_BYTECODE_FUNCTION * 256)
//#define MAX_VIRTUAL_MEMORY 134217728
/** Tamanio maximo de una cadena para ser almacenada en HASH TABLE */
#define MAX_STR_INTERN 64
/** Tamanio maximo de una cadena almacenada dinamicamente*/
#define MAX_STR_LENGTH (1024 * 1024)
/** Tamanio minimo del arreglo de frames */
#define MIN_FRAME_SIZE 8
/** Tamanio maximo de la pila de la maquina virtual */
#define MAX_STACK_SIZE (1024 * 8)
/** Tamanio maximo de una ruta de derectorio */
#define MAX_PATH_LENGTH 1024
/** Tamanio maximo de la entrada por teclado */
#define MAX_INPUT_SIZE 512
/** Maximo numero de llamadas recursivas a fun */
#define MAX_CALL_FUNCTION 27
/** Maximo numero de caracteres para un identificador */
#define MAX_ID_LENGTH 64
/** Maximo numero de bits para un buffer */
#define MAX_BUFFERSIZE BUFSIZ

#define MIN_STRTABLE_SIZE 32

#define UNUSED(x) (void)(x)

#define LAT_NUMERIC_FMT "%.16g"

#define FUNCION_VAR_ARGS (-1)

#if defined(LATINO_BUILD_AS_DLL)

#if defined(LATINO_CORE) || defined(LATINO_LIB)
#define LATINO_API __declspec(dllexport)
#else
#define LATINO_API __declspec(dllimport)
#endif

#else
#define LATINO_API extern
#endif

#define ERRLIB 1
#define ERRFUNC 2

#define latO_nulo (&latO_nulo_)
#define latO_verdadero (&latO_verdadero_)
#define latO_falso (&latO_falso_)

/* maquina virutal */
typedef struct lat_mv lat_mv;
typedef struct lat_funcion lat_funcion;

typedef void (*lat_CFuncion)(lat_mv *mv);
typedef struct lat_CReg {
    const char *nombre;
    lat_CFuncion cfun;
    int nparams;
} lat_CReg;

/*****************************************************************************/
/* API de latino */
/*****************************************************************************/

/* funciones para creacion de maquina virtual y manipulacion de la pila */
LATINO_API lat_mv *latC_crear_mv();
LATINO_API void latC_destruir_mv(lat_mv *mv);
LATINO_API lat_objeto *latC_desapilar(lat_mv *mv);
LATINO_API void latC_apilar(lat_mv *mv, lat_objeto *o);
LATINO_API lat_objeto *latC_tope(lat_mv *mv);
LATINO_API void latC_apilar_null(lat_mv *mv);
LATINO_API void latC_apilar_bool(lat_mv *mv, bool b);
LATINO_API void latC_apilar_double(lat_mv *mv, double num);
LATINO_API void latC_apilar_string(lat_mv *mv, const char *str);

/* funciones para el manejo de errores y llamado de funciones */
LATINO_API lat_objeto *latC_analizar(lat_mv *mv, ast *nodo);
LATINO_API int latC_llamar_funcion(lat_mv *mv, lat_objeto *func);
LATINO_API void latC_error(lat_mv *mv, const char *fmt, ...);

/* funciones para crear un objeto latino */
// LATINO_API lat_objeto *latC_crear_logico(lat_mv *mv, bool val);
LATINO_API lat_objeto *latC_crear_numerico(lat_mv *mv, double val);
LATINO_API lat_objeto *latC_crear_cadena(lat_mv *mv, const char *val);
LATINO_API lat_objeto *latC_crear_lista(lat_mv *mv, lista *l);
LATINO_API lat_objeto *latC_crear_dic(lat_mv *mv, hash_map *dic);
LATINO_API lat_objeto *latC_crear_cdato(lat_mv *mv, void *ptr);
LATINO_API lat_objeto *latC_crear_funcion(lat_mv *mv, lat_bytecode *inslist,
                                          int ninst);
LATINO_API lat_objeto *latC_crear_cfuncion(lat_mv *mv, lat_CFuncion func);

/* funciones para obtener el objeto y envia error
 * en caso de que no corresponda el tipo */
LATINO_API double latC_checar_numerico(lat_mv *mv, lat_objeto *o);
LATINO_API char *latC_checar_cadena(lat_mv *mv, lat_objeto *o);
LATINO_API bool latC_checar_logico(lat_mv *mv, lat_objeto *o);
LATINO_API lista *latC_checar_lista(lat_mv *mv, lat_objeto *o);
LATINO_API hash_map *latC_checar_dic(lat_mv *mv, lat_objeto *o);
LATINO_API void *latC_checar_cptr(lat_mv *mv, lat_objeto *o);

/* funciones para conversion entre tipos de dato de latino a C */
LATINO_API bool latC_abool(lat_mv *mv, lat_objeto *o);
LATINO_API double latC_adouble(lat_mv *mv, lat_objeto *o);
LATINO_API char *latC_astring(lat_mv *mv, lat_objeto *o);

/* funciones para el manejo de listas */
LATINO_API lista *latL_crear(lat_mv *mv);
LATINO_API void latL_agregar(lat_mv *mv, lista *list, void *value);
LATINO_API void latL_destruir(lat_mv *mv, lista *list);
LATINO_API void latL_limpiar(lat_mv *mv, lista *list);
LATINO_API void latL_limpiar_destruir(lat_mv *mv, lista *list);
LATINO_API void *latL_desapilar(lat_mv *mv, lista *list);
LATINO_API void latL_insertar_inicio(lat_mv *mv, lista *list, void *value);
LATINO_API void *latL_extraer_inicio(lat_mv *mv, lista *list);
LATINO_API void *latL_eliminar_nodo(lat_mv *mv, lista *list, nodo_lista *node);
LATINO_API void latL_extender(lat_mv *mv, lista *list1, lista *list2);
LATINO_API bool latL_contiene_valor(lat_mv *mv, lista *l, void *data);
LATINO_API nodo_lista *latL_obtener_nodo(lat_mv *mv, lista *list, int pos);
LATINO_API lat_objeto *latL_obtener_elemento(lat_mv *mv, lista *list, int pos);
LATINO_API void latL_insertar_elemento(lat_mv *mv, lista *list, void *data,
                                       int pos);

/* funciones para el manejo de diccionarios o hash_map*/
LATINO_API hash_map *latH_crear(lat_mv *mv);
LATINO_API void latH_asignar(lat_mv *mv, hash_map *m, const char *key,
                             void *val);
LATINO_API void latH_limpiar(lat_mv *mv, hash_map *dic);
LATINO_API void latH_destruir(lat_mv *mv, hash_map *dic);
LATINO_API void *latH_obtener(hash_map *m, const char *key);
LATINO_API hash_map *latH_clonar(lat_mv *mv, hash_map *m);

/* funciones para el manejo de librerias dinamicas */
LATINO_API void latC_abrir_liblatino(lat_mv *mv, const char *nombre_lib,
                                     const lat_CReg *funs);
LATINO_API int latC_cargarlib(lat_mv *mv, const char *path, const char *sym);

/* funciones comunes */
char *tipo(int tipo);
char *analizar_fmt(const char *s, size_t len);
char *analizar(const char *s, size_t len);
char *decimal_acadena(double d);
char *logico_acadena(int i);
bool inicia_con(const char *base, const char *str);
bool termina_con(char *base, char *str);
int ultima_pos(char *base, char *str);
char *insertar(char *dest, char *src, int pos);
char *rellenar_izquierda(char *base, char *c, int n);
char *rellenar_derecha(char *base, char *c, int n);
char *reemplazar(char *o_string, char *s_string, char *r_string);
char *subcadena(const char *str, int beg, int n);
char *minusculas(const char *str);
char *mayusculas(const char *str);
char *quitar_espacios(const char *str);

#endif /* _LATINO_H_ */
