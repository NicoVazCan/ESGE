#include "ESGE_event.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"


typedef struct ESGE_EWDataL
{
  void *userdata;
  struct ESGE_EWDataL *n;

} ESGE_EWDataL;

typedef struct ESGE_EWFuncL
{
  ESGE_EventWatch watch;
  ESGE_EWDataL *l;
  struct ESGE_EWFuncL *n;

} ESGE_EWFuncL;

typedef struct ESGE_EWTypeL
{
  Uint32 type;
  struct ESGE_EWFuncL *l;
  struct ESGE_EWTypeL *n; 

} ESGE_EWTypeL;


static ESGE_EWTypeL *ESGE_eventLoopTypes = NULL;

static ESGE_EWTypeL *ESGE_pTypeNext = NULL;
static ESGE_EWFuncL *ESGE_pFuncNext = NULL;
static ESGE_EWDataL *ESGE_pDataNext = NULL;


static void
ESGE_EventLoop()
{
  SDL_Event e;

  while (SDL_PollEvent(&e))
  {
    ESGE_EWTypeL *pType = ESGE_eventLoopTypes;

    while (pType != NULL)
    {
      ESGE_pTypeNext = pType->n;

      if (pType->type > e.type) break;
      else if (pType->type == e.type)
      {
        ESGE_EWFuncL *pFunc = pType->l;

        while (pFunc != NULL)
        {
          ESGE_EWDataL *pData = pFunc->l;
          ESGE_pFuncNext = pFunc->n;

          while (pData != NULL)
          {
            ESGE_pDataNext = pData->n;

            pFunc->watch(pData->userdata, &e);

            pData = ESGE_pDataNext;
          }

          pFunc = ESGE_pFuncNext;
        }
      }
      pType = ESGE_pTypeNext;
    }
  }
}

SDL_FORCE_INLINE int
ESGE_CmpEWDataL(ESGE_EWDataL *left, ESGE_EWDataL *right)
{
  return -(left->userdata != right->userdata);
}

SDL_FORCE_INLINE int
ESGE_CmpEWFuncL(ESGE_EWFuncL *left, ESGE_EWFuncL *right)
{
  return -(left->watch != right->watch);
}

SDL_FORCE_INLINE int
ESGE_CmpEWTypeL(ESGE_EWTypeL *left, ESGE_EWTypeL *right)
{
  return left->type - right->type;
}

int
ESGE_AddEventWatch(
  Uint32 type,
  ESGE_EventWatch watch,
  void *userdata
)
{
  if (
    ESGE_eventLoopTypes == NULL &&
    ESGE_AddPLC(ESGE_EVENT_LOOP_PRI, ESGE_EventLoop, NULL)
  )
  {
    return 1;
  }
  else
  {
    ESGE_EWDataL *pData;
    ESGE_EWFuncL *pFunc;
    ESGE_EWTypeL typeElem = {.type=type}, **ppTypePos;
    int notFound;

    SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
      ESGE_EWTypeL,
      ESGE_eventLoopTypes,
      &typeElem,
      ESGE_CmpEWTypeL,
      n,
      notFound,
      ppTypePos
    );

    if (notFound)
    {
      ESGE_EWTypeL *pType;

      if ((pData = SDL_malloc(sizeof(ESGE_EWDataL))) == NULL)
      {
        return -1;
      }
      if ((pFunc = SDL_malloc(sizeof(ESGE_EWFuncL))) == NULL)
      {
        SDL_free(pData);
        return -1;
      }
      if ((pType = SDL_malloc(sizeof(ESGE_EWTypeL))) == NULL)
      {
        SDL_free(pData);
        SDL_free(pFunc);
        return -1;
      }

      pData->userdata = userdata;
      pFunc->watch = watch;
      pFunc->l = NULL;
      pType->type = type;
      pType->l = NULL;

      SGLIB_LIST_ADD(ESGE_EWDataL, pFunc->l, pData, n);
      SGLIB_LIST_ADD(ESGE_EWFuncL, pType->l, pFunc, n);
      pType->n = *ppTypePos;
      *ppTypePos = pType;
    }
    else
    {
      ESGE_EWFuncL funcElem = {.watch=watch};

      SGLIB_LIST_FIND_MEMBER(
        ESGE_EWFuncL,
        (*ppTypePos)->l,
        &funcElem,
        ESGE_CmpEWFuncL,
        n,
        pFunc
      );

      if (pFunc == NULL)
      {
        if ((pData = SDL_malloc(sizeof(ESGE_EWDataL))) == NULL)
        {
          return -1;
        }
        if ((pFunc = SDL_malloc(sizeof(ESGE_EWFuncL))) == NULL)
        {
          SDL_free(pData);
          return -1;
        }

        pData->userdata = userdata;
        pFunc->watch = watch;
        pFunc->l = NULL;

        SGLIB_LIST_ADD(ESGE_EWDataL, pFunc->l, pData, n);
        SGLIB_LIST_ADD(ESGE_EWFuncL, (*ppTypePos)->l, pFunc, n);
      }
      else
      {
        ESGE_EWDataL dataElem = {.userdata=userdata};

        SGLIB_LIST_FIND_MEMBER(
          ESGE_EWDataL,
          pFunc->l,
          &dataElem,
          ESGE_CmpEWDataL,
          n,
          pData
        );

        if (pData == NULL)
        {
          if ((pData = SDL_malloc(sizeof(ESGE_EWDataL))) == NULL)
          {
            return -1;
          }

          pData->userdata = userdata;

          SGLIB_LIST_ADD(ESGE_EWDataL, pFunc->l, pData, n);
        }
        else return 1;
      }
    }
  }
  return 0;
}


void
ESGE_DelEventWatch(
  Uint32 type,
  ESGE_EventWatch watch,
  void *userdata
)
{
  ESGE_EWTypeL typeElem = {.type=type}, **ppTypePos;
  int notFound;

  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
    ESGE_EWTypeL,
    ESGE_eventLoopTypes,
    &typeElem,
    ESGE_CmpEWTypeL,
    n,
    notFound,
    ppTypePos
  );

  if (!notFound)
  {
    ESGE_EWFuncL funcElem = {.watch=watch}, **ppFuncPos;

    SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
      ESGE_EWFuncL,
      (*ppTypePos)->l,
      &funcElem,
      ESGE_CmpEWFuncL,
      n,
      notFound,
      ppFuncPos
    );

    if (!notFound)
    {
      ESGE_EWDataL dataElem = {.userdata=userdata}, **ppDataPos;

      SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
        ESGE_EWDataL,
        (*ppFuncPos)->l,
        &dataElem,
        ESGE_CmpEWDataL,
        n,
        notFound,
        ppDataPos
      );

      if (!notFound)
      {
        ESGE_EWDataL *pData = *ppDataPos;
        ESGE_EWFuncL *pFunc = *ppFuncPos;
        ESGE_EWTypeL *pType = *ppTypePos;

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

          if (pType->l == NULL)
          {
            if (pType == ESGE_pTypeNext) ESGE_pTypeNext = pType->n;

            *ppTypePos = pType->n;
            SDL_zerop(pType);
            SDL_free(pType);

            if (ESGE_eventLoopTypes == NULL)
            {
              ESGE_DelPLC(
                ESGE_EVENT_LOOP_PRI,
                ESGE_EventLoop,
                NULL
              );
            }
          }
        }
      }
    }
  }
}
