#include "ESGE_plc.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

typedef struct ESGE_PLCDataL
{
    void *userdata;
    struct ESGE_PLCDataL *n;

} ESGE_PLCDataL;

typedef struct ESGE_PLCFuncL
{
    ESGE_PLC func;
    ESGE_PLCDataL *l;
    struct ESGE_PLCFuncL *n;

} ESGE_PLCFuncL;

typedef struct ESGE_PLCPriL
{
    unsigned pri;
    struct ESGE_PLCFuncL *l;
    struct ESGE_PLCPriL *n; 

} ESGE_PLCPriL;

static ESGE_PLCPriL *ESGE_priGrps = NULL;

static ESGE_PLCPriL  *ESGE_pPriNext  = NULL;
static ESGE_PLCFuncL *ESGE_pFuncNext = NULL;
static ESGE_PLCDataL *ESGE_pDataNext = NULL;


SDL_INLINE SDL_bool
ESGE_AnyPLC(void) { return ESGE_priGrps != NULL; }

void
ESGE_UpdatePLC(void)
{
  ESGE_PLCPriL *pPri = ESGE_priGrps;

  while (pPri != NULL)
  {
    ESGE_PLCFuncL *pFunc = pPri->l;
    ESGE_pPriNext = pPri->n;

    while (pFunc != NULL)
    {
      ESGE_PLCDataL *pData = pFunc->l;
      ESGE_pFuncNext = pFunc->n;

      while (pData != NULL)
      {
        ESGE_pDataNext = pData->n;

        pFunc->func(pData->userdata);

        pData = ESGE_pDataNext;
      }
      pFunc = ESGE_pFuncNext;
    }
    pPri = ESGE_pPriNext;
  }
}

SDL_FORCE_INLINE int
ESGE_CmpPLCDataL(ESGE_PLCDataL *pPD0, ESGE_PLCDataL *pPD1)
{
  return -(pPD0->userdata != pPD1->userdata);
}

SDL_FORCE_INLINE int
ESGE_CmpPLCFuncL(ESGE_PLCFuncL *pCG0, ESGE_PLCFuncL *pCG1)
{
  return -(pCG0->func != pCG1->func);
}

SDL_FORCE_INLINE int
ESGE_CmpPLCPriL(ESGE_PLCPriL *pPri0, ESGE_PLCPriL *pPri1)
{
  return pPri0->pri - pPri1->pri;
}

int 
ESGE_AddPLC(unsigned pri, ESGE_PLC func, void *userdata)
{
  ESGE_PLCDataL *pData;
  ESGE_PLCFuncL *pFunc;
  ESGE_PLCPriL priElem = {.pri=pri}, **ppPriPos;
  int notFound;

  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
    ESGE_PLCPriL,
    ESGE_priGrps,
    &priElem,
    ESGE_CmpPLCPriL,
    n,
    notFound,
    ppPriPos
  );

  if (notFound)
  {
    ESGE_PLCPriL *pPri;

    if ((pData = SDL_malloc(sizeof(ESGE_PLCDataL))) == NULL)
    {
      return -1;
    }
    if ((pFunc = SDL_malloc(sizeof(ESGE_PLCFuncL))) == NULL)
    {
      SDL_free(pData);
      return -1;
    }
    if ((pPri = SDL_malloc(sizeof(ESGE_PLCPriL))) == NULL)
    {
      SDL_free(pPri);
      return -1;
    }

    pData->userdata = userdata;
    pFunc->func = func;
    pFunc->l = NULL;
    pPri->pri = pri;
    pPri->l = NULL;

    SGLIB_LIST_ADD(ESGE_PLCDataL, pFunc->l, pData, n);
    SGLIB_LIST_ADD(ESGE_PLCFuncL, pPri->l, pFunc, n);
    pPri->n = *ppPriPos;
    *ppPriPos = pPri;
  }
  else
  {
    ESGE_PLCFuncL funcElem = {.func=func};

    SGLIB_LIST_FIND_MEMBER(
      ESGE_PLCFuncL,
      (*ppPriPos)->l,
      &funcElem,
      ESGE_CmpPLCFuncL,
      n,
      pFunc
    );

    if (pFunc == NULL)
    {
      if ((pData = SDL_malloc(sizeof(ESGE_PLCDataL))) == NULL)
      {
        return -1;
      }
      if ((pFunc = SDL_malloc(sizeof(ESGE_PLCFuncL))) == NULL)
      {
        SDL_free(pData);
        return -1;
      }

      pData->userdata = userdata;
      pFunc->func = func;
      pFunc->l = NULL;

      SGLIB_LIST_ADD(ESGE_PLCDataL, pFunc->l, pData, n);
      SGLIB_LIST_ADD(ESGE_PLCFuncL, (*ppPriPos)->l, pFunc, n);
    }
    else
    {
      ESGE_PLCDataL dataElem = {.userdata=userdata};

      SGLIB_LIST_FIND_MEMBER(
        ESGE_PLCDataL,
        pFunc->l,
        &dataElem,
        ESGE_CmpPLCDataL,
        n,
        pData
      );

      if (pData == NULL)
      {
        if ((pData = SDL_malloc(sizeof(ESGE_PLCDataL))) == NULL)
        {
          return -1;
        }

        pData->userdata = userdata;

        SGLIB_LIST_ADD(ESGE_PLCDataL, pFunc->l, pData, n);
      }
      else return 1;
    }
  }

  return 0;
}

void
ESGE_DelPLC(unsigned pri, ESGE_PLC func, void *userdata)
{
  ESGE_PLCPriL priElem = {.pri=pri}, **ppPriPos;
  int notFound;

  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
    ESGE_PLCPriL,
    ESGE_priGrps,
    &priElem,
    ESGE_CmpPLCPriL,
    n,
    notFound,
    ppPriPos
  );

  if (!notFound)
  {
    ESGE_PLCFuncL funcElem = {.func=func}, **ppFuncPos;

    SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
      ESGE_PLCFuncL,
      (*ppPriPos)->l,
      &funcElem,
      ESGE_CmpPLCFuncL,
      n,
      notFound,
      ppFuncPos
    );

    if (!notFound)
    {
      ESGE_PLCDataL dataElem = {.userdata=userdata}, **ppDataPos;

      SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
        ESGE_PLCDataL,
        (*ppFuncPos)->l,
        &dataElem,
        ESGE_CmpPLCDataL,
        n,
        notFound,
        ppDataPos
      );

      if (!notFound)
      {
        ESGE_PLCDataL *pData = *ppDataPos;
        ESGE_PLCFuncL *pFunc = *ppFuncPos;
        ESGE_PLCPriL  *pPri  = *ppPriPos;

        if (pData == ESGE_pDataNext) ESGE_pDataNext = pData->n;

        *ppDataPos = pData->n;
        SDL_zerop(pData);
        SDL_free(pData);

        if (pFunc->l == NULL)
        {
          if (pFunc == ESGE_pFuncNext) ESGE_pFuncNext = pFunc->n;

          *ppFuncPos = pFunc->n;
          SDL_zerop(pFunc);
          SDL_free(pFunc);

          if (pPri->l == NULL)
          {
            if (pPri == ESGE_pPriNext) ESGE_pPriNext = pPri->n;

            *ppPriPos = pPri->n;
            SDL_zerop(pPri);
            SDL_free(pPri);
          }
        }
      }
    }
  }
}
