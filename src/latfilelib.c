/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#include "latino.h"

#define LIB_ARCHIVO_NAME "archivo"

static size_t file_leerlinea(char **lineptr, size_t *n, FILE *stream) {
    static char line[256];
    char *ptr;
    size_t len;
    if (lineptr == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (ferror(stream)) {
        return -1;
    }
    if (feof(stream)) {
        return -1;
    }
    fgets(line, 256, stream);
    ptr = strchr(line, '\n');
    if (ptr) {
        *ptr = '\0';
    } else {
        return -1;
    }
    len = strlen(line);
    if ((len + 1) < 256) {
        ptr = realloc(*lineptr, 256);
        *lineptr = ptr;
        *n = 256;
    }
    strcpy(*lineptr, line);
    return (len);
}

static void file_lineas(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    FILE *fp;
    char *buf = NULL;
    size_t len = 0;
    char *path = latC_checar_cadena(mv, o);
    fp = fopen(path, "r");
    if (fp == NULL) {
        latC_error(mv, "No se pudo abrir el archivo '%s'", path);
    }
    lat_objeto *lineas = latC_crear_lista(mv, latL_crear(mv));
    while ((len = file_leerlinea(&buf, &len, fp)) != (size_t)-1) {
        latL_agregar(mv, latC_checar_lista(mv, lineas),
                     latC_crear_cadena(mv, buf));
    }
    fclose(fp);
    latC_apilar(mv, lineas);
}

static void file_leer(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    FILE *archivo = fopen(latC_checar_cadena(mv, o), "r");
    if (archivo == NULL) {
        latC_apilar(mv, latO_falso);
        return;
    }
    char *final;
    size_t n = 0;
    int c;
    fseek(archivo, 0, SEEK_END);
    long f_size = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    final = malloc(f_size + 1);
    while ((c = fgetc(archivo)) != EOF) {
        final[n++] = (char)c;
    }
    final[n] = '\0';
    fclose(archivo);
    latC_apilar(mv, latC_crear_cadena(mv, final));
    free(final);
}

static void file_escribir(lat_mv *mv) {
    lat_objeto *s = latC_desapilar(mv);
    lat_objeto *o = latC_desapilar(mv);
    if (o->tipo == T_STR) {
        FILE *fp;
        fp = fopen(latC_checar_cadena(mv, o), "w");
        const char *cad = latC_checar_cadena(mv, s);
        size_t lon = strlen(cad);
        fwrite(cad, 1, lon, fp);
        fclose(fp);
    } else {
        latC_error(mv, "No se pudo poner en el archivo '%s'",
                   latC_checar_cadena(mv, o));
    }
}

static void file_ejecutar(lat_mv *mv) {
    lat_objeto *o = latC_desapilar(mv);
    char *input = latC_checar_cadena(mv, o);
    char *dot = strrchr(input, '.');
    char *extension;
    if (!dot || dot == input) {
        extension = "";
    } else {
        extension = dot + 1;
    }
    if (!strcmp(extension, "lat")) {
        int status;
        ast *nodo = latA_analizar_arch(input, &status);
        if (!nodo) {
            latC_error(mv, "Error al leer el archivo: '%s'", input);
        }
        mv->global->REPL = false;
        mv->nombre_archivo = input;
        lat_objeto *func = latC_analizar(mv, nodo);
        if (status == 0) {
            status = latC_llamar_funcion(mv, func);
            latA_destruir(nodo);
        } else {
            latC_error(mv, "Error al ejeuctar archivo '%s'", input);
        }
    }
}

static void file_copiar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    FILE *archivo = fopen(latC_checar_cadena(mv, a), "a");
    fprintf(archivo, "%s", latC_checar_cadena(mv, b));
    fclose(archivo);
}

static void file_eliminar(lat_mv *mv) {
    int status;
    lat_objeto *a = latC_desapilar(mv);

    status = remove(latC_checar_cadena(mv, a));
    if (status == 0) {
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

static void file_crear(lat_mv *mv) {
    lat_objeto *a = latC_desapilar(mv);
    FILE *archivo;
    archivo = fopen(latC_checar_cadena(mv, a), "r");
    if (archivo == NULL) {
        archivo = fopen(latC_checar_cadena(mv, a), "wb");
        latC_apilar(mv, latO_verdadero);
    } else {
        latC_apilar(mv, latO_falso);
    }
}

static void file_renombrar(lat_mv *mv) {
    lat_objeto *b = latC_desapilar(mv);
    lat_objeto *a = latC_desapilar(mv);
    char *nuevo = latC_checar_cadena(mv, b);
    bool ret = rename(latC_checar_cadena(mv, a), nuevo);
    if (!ret) {
        latC_apilar(mv, latC_crear_cadena(mv, nuevo));
    } else {
        latC_apilar(mv, latO_falso);
    }
    free(nuevo);
}

static const lat_CReg libfile[] = {{"leer", file_leer, 1},
                                   {"lineas", file_lineas, 1},
                                   {"ejecutar", file_ejecutar, 1},
                                   {"poner", file_escribir, 2},
                                   {"copiar", file_copiar, 2},
                                   {"eliminar", file_eliminar, 1},
                                   {"crear", file_crear, 1},
                                   {"renombrar", file_renombrar, 2},
                                   {NULL, NULL, 0}};

void latC_abrir_liblatino_filelib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_ARCHIVO_NAME, libfile);
}
