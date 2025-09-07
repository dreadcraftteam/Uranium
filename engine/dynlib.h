// Header Dynamic library Loader
// By ThePixelMoon
//

#ifndef DYNLIB_H
#define DYNLIB_H

typedef struct DynLib DynLib;

DynLib* dynlib_open(const char* path);
void* dynlib_symbol(DynLib* lib, const char* symbol);
void  dynlib_close(DynLib* lib);

#define LOAD_FN(lib, name) \
	void (*name)() = (void (*)())dynlib_symbol(lib, #name)

#endif // DYNLIB_H

