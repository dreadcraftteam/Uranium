// Dynamic library Loader
// By ThePixelMoon
//

#include "stdlib.h"
#include "dynlib.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

struct DynLib
{
    HMODULE handle;
};

DynLib* dynlib_open(const char* path)
{
    HMODULE handle = LoadLibraryA(path);
    if (!handle) return NULL;

    DynLib* lib = malloc(sizeof(DynLib));
    lib->handle = handle;
    return lib;
}

void* dynlib_symbol(DynLib* lib, const char* symbol)
{
    return (void*)GetProcAddress(lib->handle, symbol);
}

void dynlib_close(DynLib* lib)
{
    if (!lib)
		return;
    FreeLibrary(lib->handle);
    free(lib);
}
#else
#include <dlfcn.h>

struct DynLib
{
    void* handle;
};

DynLib* dynlib_open(const char* path)
{
    void* handle = dlopen(path, RTLD_LAZY);
    if (!handle) return NULL;

    DynLib* lib = malloc(sizeof(DynLib));
    lib->handle = handle;
    return lib;
}

void* dynlib_symbol(DynLib* lib, const char* symbol)
{
    return dlsym(lib->handle, symbol);
}

void dynlib_close(DynLib* lib)
{
    if (!lib)
		return;
    
	dlclose(lib->handle);
    free(lib);
}
#endif

