#include "REF.h"


typedef struct REF
{
  void **ppvOut;
  struct REF *pkNext;
} REF;


inline void
REF_Init(REF_List *pkThis, void *pvIn)
{
  pkThis->pvIn   = pvIn;
  pkThis->pkRefs = NULL;
}

int
REF_Add(REF_List *pkThis, void **ppvOut)
{
	REF *pkElem;

	if (ppvOut == NULL) return 1;

	pkElem = REF_MALLOC(sizeof(REF));

	if (pkElem == NULL) return 1;

	pkElem->ppvOut = ppvOut;
	SGLIB_LIST_ADD(REF, pkThis->pkRefs, pkElem, pkNext);

	*ppvOut = pkThis->pvIn;

	return 0;
}

static int
REF_Cmp(REF *pkL, REF *pkR)
{
  return pkL->ppvOut - pkR->ppvOut;
}

void
REF_Remove(REF_List *pkThis, void **ppvOut)
{
  REF kElem = {.ppvOut=ppvOut}, *pkMem;

  SGLIB_LIST_DELETE_IF_MEMBER(
    REF,
    pkThis->pkRefs,
    &kElem,
    REF_Cmp,
    pkNext,
    pkMem
  );
  REF_FREE(pkMem);
}

void
REF_Close(REF_List *pkThis)
{
  SGLIB_LIST_MAP_ON_ELEMENTS(
    REF,
    pkThis->pkRefs,
    pkElem,
    pkNext,
    {
      *pkElem->ppvOut = NULL;

      pkElem->ppvOut = NULL;
      pkElem->pkNext = NULL;
      REF_FREE(pkElem);
    }
  );
  pkThis->pvIn = NULL;
  pkThis->pkRefs = NULL;
}
