#include "ESGE_objStatic.h"

#include <SDL2/SDL.h>


ESGE_ObjStatic::ESGE_ObjStaticList *ESGE_ObjStatic::hList = NULL;


ESGE_ObjStatic*
ESGE_ObjStatic::GetObjAt(SDL_Point pos)
{
  unsigned col, row;
  ESGE_ObjStaticList *hNode;

  col = ((unsigned)pos.x)/ESGE_OBJ_STATIC_W;
  row = ((unsigned)pos.y)/ESGE_OBJ_STATIC_H;

  for (
    hNode = hList;
    (
      hNode != NULL &&
      ((unsigned)hNode->vList->pos.x) / ESGE_OBJ_STATIC_W < col
    );
    hNode = hNode->next
  );

  if (
    hNode != NULL &&
    ((unsigned)hNode->vList->pos.x) / ESGE_OBJ_STATIC_W == col
  )
  {
    ESGE_ObjStatic *vNode;

    for (
      vNode = hNode->vList;
      (
        vNode != NULL &&
        ((unsigned)vNode->pos.y) / ESGE_OBJ_STATIC_H < row
      );
      vNode = vNode->next
    );

    if (
      vNode != NULL &&
      ((unsigned)vNode->pos.y) / ESGE_OBJ_STATIC_H == row
    )
      return vNode;
  }
  return NULL;
}


ESGE_ObjStatic::ESGE_ObjStatic(void)
{
  offsetSize.x = 0;
  offsetSize.y = 0;
  offsetSize.w = ESGE_OBJ_STATIC_W;
  offsetSize.h = ESGE_OBJ_STATIC_H;
}

ESGE_ObjStatic::~ESGE_ObjStatic(void)
{}


void
ESGE_ObjStatic::EnableStatic(void)
{
  unsigned col, row;
  ESGE_ObjStaticList **hNode;
  ESGE_ObjStatic **vNode;

  enabledStatic = true;

  col = ((unsigned)pos.x)/ESGE_OBJ_STATIC_W;
  row = ((unsigned)pos.y)/ESGE_OBJ_STATIC_H;

  for (
    hNode = &hList;
    (
      *hNode != NULL &&
      ((unsigned)(*hNode)->vList->pos.x) / ESGE_OBJ_STATIC_W < col
    );
    hNode = &(*hNode)->next
  );

  if (
    *hNode == NULL ||
    ((unsigned)(*hNode)->vList->pos.x) / ESGE_OBJ_STATIC_W != col
  )
    *hNode = new ESGE_ObjStaticList{NULL, *hNode};

  for (
    vNode = &(*hNode)->vList;
    (
      *vNode != NULL &&
      ((unsigned)(*vNode)->pos.y) / ESGE_OBJ_STATIC_H < row
    );
    vNode = &(*vNode)->next
  );

  SDL_assert(
    *vNode == NULL ||
    ((unsigned)(*vNode)->pos.y) / ESGE_OBJ_STATIC_H != row
  );

  next = *vNode;
  *vNode = this;
}

void
ESGE_ObjStatic::DisableStatic(void)
{
  unsigned col;
  ESGE_ObjStaticList **hNode;
  ESGE_ObjStatic **vNode;

  enabledStatic = false;

  col = ((unsigned)pos.x)/ESGE_OBJ_STATIC_W;

  for (
    hNode = &hList;
    (
      *hNode != NULL &&
      ((unsigned)(*hNode)->vList->pos.x) / ESGE_OBJ_STATIC_W < col
    );
    hNode = &((*hNode)->next)
  );

  if (
    *hNode != NULL &&
    ((unsigned)(*hNode)->vList->pos.x) / ESGE_OBJ_STATIC_W == col
  )
  {
    for (
      vNode = &((*hNode)->vList);
      *vNode != NULL && *vNode != this;
      vNode = &((*vNode)->next)
    );

    if (*vNode != NULL && *vNode != this)
    {
      *vNode = (*vNode)->next;

      if ((*hNode)->vList == NULL)
      {
        ESGE_ObjStaticList *hNodeNext = (*hNode)->next;
        delete *hNode;
        *hNode = hNodeNext;
      }
    }
  }
}

bool
ESGE_ObjStatic::IsEnabledStatic(void)
{
  return enabledStatic;
}
