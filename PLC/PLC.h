#ifndef PLC_H_
#define PLC_H_
#endif

#ifdef PLC_ASSERT
#  define SGLIB_ASSERT PLC_ASSERT
#endif
#include <sglib.h>

#ifndef PLC_MALLOC
#  ifdef PLC_FREE
#    undef PLC_FREE
#  endif
#  include <stdlib.h>
#  define PLC_MALLOC malloc
#  define PLC_FREE free
#elif !defined(PLC_FREE)
#  ifdef PLC_MALLOC
#    undef PLC_MALLOC
#  endif
#  include <stdlib.h>
#  define PLC_MALLOC malloc
#  define PLC_FREE free
#endif

#ifndef NULL
#  include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PLC_PriGrps PLC_PriGrps;
typedef void (*PLC_Callback)(PLC_PriGrps**, void*);

void PLC_init(PLC_PriGrps **pL);
int PLC_empty(const PLC_PriGrps *l);
int PLC_add(PLC_PriGrps **pL, unsigned pri, PLC_Callback f, void *d);
int PLC_remove(PLC_PriGrps **pL, unsigned pri, PLC_Callback f, void *d);
void PLC_update(PLC_PriGrps **pL);

#ifdef __cplusplus
}

#endif
