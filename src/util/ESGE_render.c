#include "ESGE_render.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

#define CLEAR_R 0x00
#define CLEAR_G 0x00
#define CLEAR_B 0x00
#define CLEAR_A 0xFF

#define CMP_REND(left, right) (     \
  -((left)->rend != (right)->rend)  \
)
#define CMP_REND_DATA(left, right) ( \
  (left)->layer == (right)->layer? -( \
    (left)->userdata != (right)->userdata || \
    (left)->draw != (right)->draw \
  ) : \
  (int)(left)->layer - (int)(right)->layer \
)

typedef struct ESGE_RendData
{
  unsigned layer;
  void *userdata;
  ESGE_DrawCallback draw;
  struct ESGE_RendData *next;
} ESGE_RendData;

typedef struct ESGE_Rend
{
  SDL_Renderer *rend;
  struct ESGE_RendData *list;
  struct ESGE_Rend *next;
} ESGE_Rend;

static ESGE_Rend      *ESGE_rendList    = NULL;

static ESGE_Rend      *ESGE_rendNext      = NULL;
static ESGE_RendData  *ESGE_rendDataNext  = NULL;

typedef struct ESGE_Renderers
{
  SDL_Renderer *array;
  size_t n;
} ESGE_Renderers;

static ESGE_Renderers ESGE_renders = {NULL, 0};

int
ESGE_AddRenderer(SDL_Renderer *render)
{
  size_t chunks = 0;
  void *new_ptr;

  for (
    ;
    chunks < sizeof (size_t) * 8 && ESGE_renders >> chunks > 1;
    ++chunks
  )

  ESGE_renders *= 2;

  new_ptr = SDL_realloc(
    ESGE_renders.array,
    sizeof (SDL_Renderer*) * ESGE_renders.n
  );

  if (new_ptr == NULL) return 1;
  else
  {
    ESGE_renders.array = new_ptr;
    return 0;
  }
}

void
ESGE_DelRenderer(SDL_Renderer *render)
{

}

static void
ESGE_DrawLayers()
{
  ESGE_Rend *pRend = ESGE_rendList;

  while (pRend != NULL)
  {
    ESGE_RendData *pData = pRend->list;

    ESGE_rendNext = pRend->next;

    if (
      SDL_SetRenderDrawColor(
        pRend->rend,
        CLEAR_R,
        CLEAR_G,
        CLEAR_B,
        CLEAR_A
      )
    )
    {
      SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
      SDL_ClearError();
    }
    if (SDL_RenderClear(pRend->rend))
    {
      SDL_LogError(SDL_LOG_CATEGORY_RENDER, "%s", SDL_GetError());
      SDL_ClearError();
    }

    while (pData != NULL)
    {
      ESGE_rendDataNext = pData->next;

      pData->draw(pData->userdata, pRend->rend);

      pData = ESGE_rendDataNext;
    }

    SDL_RenderPresent(pRend->rend);

    pRend = ESGE_rendNext;
  }
}

int
ESGE_AddDrawCallback(
  unsigned layer,
  SDL_Renderer *rend,
  ESGE_DrawCallback draw,
  void *userdata
)
{
  if (
    ESGE_rendList == NULL &&
    ESGE_AddPLC(ESGE_DRAW_LAYERS_PRI, ESGE_DrawLayers, NULL)
  )
  {
    return 1;
  }
  else
  {
    ESGE_RendData  dataElem = {
      .layer=layer,
      .userdata=userdata,
      .draw=draw
    }, *pData, **ppDataPos;
    ESGE_Rend      rendElem = {.rend=rend, .list=NULL}, *pRend;
    int notFound;

    SGLIB_LIST_FIND_MEMBER(
      ESGE_Rend,
      ESGE_rendList,
      &rendElem,
      CMP_REND,
      next,
      pRend
    );    

    if (pRend == NULL)
    {
      if ((pData = SDL_malloc(sizeof(ESGE_RendData))) == NULL)
      {
        return -1;
      }
      if ((pRend = SDL_malloc(sizeof(ESGE_Rend))) == NULL)
      {
        SDL_free(pData);
        return -1;
      }
      *pData  = dataElem;
      *pRend  = rendElem;

      SGLIB_SORTED_LIST_ADD(
        ESGE_RendData,
        pRend->list,
        pData,
        CMP_REND_DATA,
        next
      );
      SGLIB_LIST_ADD(ESGE_Rend, ESGE_rendList, pRend, next);
    }
    else
    {
      SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
        ESGE_RendData,
        pRend->list,
        &dataElem,
        CMP_REND_DATA,
        next,
        notFound,
        ppDataPos
      );

      if (notFound)
      {
        if ((pData = SDL_malloc(sizeof(ESGE_RendData))) == NULL)
        {
          return -1;
        }
        *pData  = dataElem;

        pData->next = *ppDataPos;
        *ppDataPos = pData;
      }
      else return -1;
    }
  }
  return 0;
}


void
ESGE_DelDrawCallback(
  unsigned layer,
  SDL_Renderer *rend,
  ESGE_DrawCallback draw,
  void *userdata
)
{
  ESGE_Rend rendElem = {.rend=rend}, **ppRendPos;

  int notFound;

  SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
    ESGE_Rend,
    ESGE_rendList,
    &rendElem,
    CMP_REND,
    next,
    notFound,
    ppRendPos
  );

  if (!notFound)
  {
    ESGE_RendData dataElem = {
      .layer=layer,
      .userdata=userdata,
      .draw=draw
    }, **ppDataPos;

    SGLIB_SORTED_LIST_FIND_MEMBER_OR_PLACE(
      ESGE_RendData,
      (*ppRendPos)->list,
      &dataElem,
      CMP_REND_DATA,
      next,
      notFound,
      ppDataPos
    );

    if (!notFound)
    {
      ESGE_RendData *pData   = *ppDataPos;
      ESGE_Rend *pRend       = *ppRendPos;

      if (pData == ESGE_rendDataNext)
      {
        ESGE_rendDataNext = pData->next;
      }
      *ppDataPos = pData->next;
      SDL_zerop(pData);
      SDL_free(pData);

      if (pRend->list == NULL)
      {
        if (pRend == ESGE_rendNext)
        {
          ESGE_rendNext = pRend->next;
        }
        *ppRendPos = pRend->next;
        SDL_zerop(pRend);
        SDL_free(pRend);

        if (ESGE_rendList == NULL)
        {
          ESGE_DelPLC(
            ESGE_DRAW_LAYERS_PRI,
            ESGE_DrawLayers,
            NULL
          );
        }
      }
    }
  }
}
