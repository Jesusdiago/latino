/*
The MIT License (MIT)
Vea LICENSE.txt
 */

#define LATINO_LIB

#include <sys/stat.h>

#include "latino.h"

#define LIB_PAQ_NAME "paquete"

/*static void lat_unloadlib(void *lib) { dlclose(lib); }*/

#ifdef _WIN32
static void throw_error(lat_mv *mv) {
    int error = GetLastError();
    char buffer[128];
    if (FormatMessageA(FORMAT_MESSAGE_IGNORE_INSERTS |
                           FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL, error, 0, buffer, sizeof(buffer), NULL))
        latC_error(mv, buffer);
    else {
        latC_error(mv, "error del sistema");
    }
}
static void *lat_load(lat_mv *mv, const char *path) {
    HINSTANCE lib = LoadLibraryA(path);
    if (lib == NULL) {
        throw_error(mv);
    }
    return lib;
}

static lat_CFuncion lat_sym(lat_mv *mv, void *lib, const char *sym) {
    lat_CFuncion f = (lat_CFuncion)GetProcAddress((HINSTANCE)lib, sym);
    if (f == NULL) {
        throw_error(mv);
    }
    return f;
}
#else
static void *lat_load(lat_mv *mv, const char *path) {
    void *lib = dlopen(path, RTLD_NOW);
    if (lib == NULL) {
        latC_error(mv, dlerror());
    }
    return lib;
}

static lat_CFuncion lat_sym(lat_mv *mv, void *lib, const char *sym) {
    lat_CFuncion f = (lat_CFuncion)dlsym(lib, sym);
    if (f == NULL) {
        latC_error(mv, dlerror());
    }
    return f;
}
#endif //  WIN32

LATINO_API int latC_cargarlib(lat_mv *mv, const char *path, const char *sym) {
    void *handle = lat_load(mv, path);
    if (handle == NULL) {
        return ERRLIB;
    } else {
        // dlerror();
        lat_CFuncion f = lat_sym(mv, handle, sym);
        if (f == NULL) {
            return ERRFUNC;
        }
        lat_objeto *cf = latC_crear_cfuncion(mv, f);
        return latC_llamar_funcion(mv, cf);
    }
}

static void latP_cargar(lat_mv *mv) {
    const char *lib = latC_checar_cadena(mv, latC_desapilar(mv));
    const char *path = latC_checar_cadena(mv, latC_desapilar(mv));
    int stat = latC_cargarlib(mv, path, lib);
    if (stat == 0)
        return;
    else {
        latC_error(mv, "Error al cargar libreria dinamica '%s'", lib);
        return;
    }
}

static const lat_CReg libpaq[] = {{"cargar", latP_cargar, 2}, {NULL, NULL, 0}};

void latC_abrir_liblatino_paqlib(lat_mv *mv) {
    latC_abrir_liblatino(mv, LIB_PAQ_NAME, libpaq);
}
