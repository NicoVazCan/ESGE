#ifndef REF_H_
#define REF_H_
#endif

#ifdef REF_ASSERT
#  define SGLIB_ASSERT REF_ASSERT
#endif
#include <sglib.h>

#ifndef REF_MALLOC
#  ifdef REF_FREE
#    undef REF_FREE
#  endif
#  include <stdlib.h>
#  define REF_MALLOC malloc
#  define REF_FREE free
#elif !defined(REF_FREE)
#  ifdef REF_MALLOC
#    undef REF_MALLOC
#  endif
#  include <stdlib.h>
#  define REF_MALLOC malloc
#  define REF_FREE free
#endif

#ifndef NULL
#  include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct REF_List
{
  void *pvIn;
  struct REF *pkRefs;
} REF_List;

void REF_Init(REF_List *pkThis, void *pvIn);
int  REF_Add(REF_List *pkThis, void **ppvOut);
void REF_Remove(REF_List *pkThis, void **ppvOut);
void REF_Close(REF_List *pkThis);

#ifdef __cplusplus
}

#endif