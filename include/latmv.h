/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#ifndef _LATINO_MV_H_
#define _LATINO_MV_H_

#include <math.h>

#include "latdic.h"
#include "latlist.h"
#include "latobj.h"

#include "latino.h"

typedef struct lat_bytecode lat_bytecode;
typedef struct lat_funcion lat_funcion;
typedef struct lat_CReg lat_CReg;
typedef void (*lat_CFuncion)(lat_mv *mv);

/**\brief Muestra las instrucciones que se van ejecutando con la Maquina Virtual
 */
#define DEPURAR_MV 0

/**\brief op_codes de la maquina virtual */
#define NOP 0
#define HALT 1
#define UNARY_MINUS 2
#define BINARY_ADD 3
#define BINARY_SUB 4
#define BINARY_MUL 5
#define BINARY_DIV 6
#define BINARY_MOD 7
#define OP_GT 8
#define OP_GE 9
#define OP_LT 10
#define OP_LE 11
#define OP_EQ 12
#define OP_NEQ 13
#define OP_AND 14
#define OP_OR 15
#define OP_NOT 16
#define OP_INC 17
#define OP_DEC 18
#define CONCAT 19
#define LOAD_CONST 20
#define LOAD_NAME 21
#define STORE_NAME 22
#define JUMP_ABSOLUTE 23
#define POP_JUMP_IF_FALSE 24
#define POP_JUMP_IF_TRUE 25 /* No se usa */
#define PUSH_CTX 26
#define POP_CTX 27
#define CALL_FUNCTION 28
#define RETURN_VALUE 29
#define MAKE_FUNCTION 30
#define LOAD_ATTR 31
#define BUILD_LIST 32
#define STORE_SUBSCR 33
#define BINARY_SUBSCR 34
#define BUILD_MAP 35
#define STORE_MAP 36
#define STORE_ATTR 37
#define SET_GLOBAL 38
#define OP_REGEX 39
#define BINARY_POW 40
#define OP_VAR_ARGS 41
#define OP_PUSH 42 /* No se usa */
#define OP_POP 43  /* No se usa */
#define ADJUST_STACK 44
#define LOAD_VAR_ARGS 45
#define SET_LOCAL 46

union lat_gcobjeto {
    lat_gcheader gch;
    union lat_cadena cadena;
    struct lista lista;
    struct hash_map dic;
    struct lat_funcion fun;
    lat_CFuncion *cfun;
    void *ptr;
};

typedef struct lat_proto {
    lat_commonheader;
    int nparams;
    int ninst;
    char *nombre;
    lat_bytecode *codigo;
    lista *locals;
} lat_proto;

typedef struct stringtable {
    lat_gcobjeto **hash;
    unsigned int nuse;
    int size;
} stringtable;

typedef struct lat_global {
    int argc;
    lat_objeto *argv;
    lat_objeto *consts;
    lat_objeto *gc_objetos;
    stringtable strt;
    bool menu;
    bool REPL;
} lat_global;

typedef struct lat_frame {
    lat_objeto *base;
    lat_objeto *func;
    lat_objeto *top;
    int nresults;
} lat_frame;

/**\brief Define la maquina virtual (MV) */
/*typedef struct lat_mv {
    lat_global *global;
    lat_objeto *pila;
    lat_objeto *tope;
    lat_objeto *base;
    lat_objeto *actfun;
    lat_objeto *contexto[256];
    lat_objeto *contexto_actual;
    lat_frame *end_frm;
    lat_frame *base_frm;
    int ptrctx;
    int ptrpila;
    int ptrprevio;
    int prev_args;
    int numejec;
    size_t memoria_usada;
    size_t gc_limite;
    char *nombre_archivo;
    int nlin;
    int ncol;
    int status;
    struct lat_longjmp *error;
} lat_mv;*/

typedef struct lat_mv {
    lat_global *global;
    lat_frame *actual_frm;
    lat_frame *final_frm;
    lat_frame *base_frm;
    lat_objeto **constantes;
    lat_objeto *tope;
    lat_objeto *base;
    lat_objeto *pila_ultimo;
    lat_objeto *pila;
    lat_objeto *contexto[256];
    lat_objeto *contexto_actual;
    const lat_bytecode *tmp_pc;
    size_t memoria_usada;
    size_t gc_limite;
    int ptrctx;
    int ptrpila;
    int ptrprevio;
    int prev_args;
    int numejec;
    int nlin;
    int ncol;
    int status;
    int size_frm;
    char *nombre_archivo;
    struct lat_longjmp *error;
} lat_mv;

#define lati_numUmin(a) (-(a))
#define lati_numAdd(a, b) ((a) + (b))
#define lati_numSub(a, b) ((a) - (b))
#define lati_numMul(a, b) ((a) * (b))
#define lati_numDiv(a, b) ((a) / (b))
#define lati_numMod(a, b) (fmod((a), (b)))
#define lati_numPow(a, b) (pow((a), (b)))
#define lati_numNq(a, b) ((a) == (b))
#define lati_numLt(a, b) ((a) < (b))
#define lati_numLe(a, b) ((a) <= (b))

#define arith_op(op)                                                           \
    {                                                                          \
        lat_objeto *b = latC_desapilar(mv);                                    \
        lat_objeto *a = latC_desapilar(mv);                                    \
        setNumerico(mv->tope, op(latC_adouble(mv, a), latC_adouble(mv, b)));   \
        inc_pila(mv);                                                          \
    }

const char *latMV_bytecode_nombre(int inst);

void lat_es_decimal(lat_mv *mv);
void lat_formato_numero(lat_mv *mv);
lat_bytecode latMV_bytecode_crear(int i, int a, int b, void *meta,
                                  long int nlin, long int ncol,
                                  char *nombre_archivo);
int latMV_funcion_correr(lat_mv *mv, lat_objeto *func);

#endif /* _LATINO_MV_H_ */
