#include "ESGE_objStatic.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

ESGE_ObjStatic *ESGE_ObjStatic::list = NULL;

ESGE_ObjStatic::ESGE_ObjStatic(SDL_Point pos, SDL_Rect colBox):
  ESGE_ObjPoint(pos),
  ESGE_ObjCollider(pos, colBox)
{
  SDL_Point ceil;
  ESGE_ObjStaticHNode **hNode;
  ESGE_ObjStaticVNode **vNode;
  ESGE_ObjStaticList *chunk;

  ceil.x = pos.x/ESGE_OBJ_STATIC_H_RES;
  ceil.y = pos.y/ESGE_OBJ_STATIC_V_RES;

  for (
    hNode = &hList;
    *hNode != NULL && (*hNode)->col < ceil.x;
    hNode = &((*hNode)->next)
  );

  if (*hNode == NULL || (*hNode)->col != ceil.x)
  {
    *hNode = new ESGE_ObjStaticHNode{ceil.x, NULL, *hNode};
  }

  for (
    vNode = &((*hNode)->vList);
    *vNode != NULL && (*vNode)->row < ceil.y;
    vNode = &((*vNode)->next)
  );

  if (*vNode == NULL && (*vNode)->row != ceil.y)
  {
    *vNode = new ESGE_ObjStaticVNode{ceil.y, NULL, *vNode};
  }

  chunk = new ESGE_ObjStaticList{this, NULL};

  SGLIB_LIST_ADD(
    ESGE_ObjStaticList,
    (*vNode)->objList,
    chunk,
    next
  );

  SGLIB_LIST_ADD(
    ESGE_ObjStatic,
    list,
    this,
    next
  );
}

ESGE_ObjStatic::~ESGE_ObjStatic(void)
{
  SDL_Point ceil;
  ESGE_ObjStaticHNode **hNode;
  ESGE_ObjStaticVNode **vNode;
  ESGE_ObjStaticList **objNode;

  ceil.x = pos.x/ESGE_OBJ_STATIC_H_RES;
  ceil.y = pos.y/ESGE_OBJ_STATIC_V_RES;

  for (
    hNode = &hList;
    *hNode != NULL && (*hNode)->col < ceil.x;
    hNode = &((*hNode)->next)
  );

  if (*hNode != NULL && (*hNode)->col == ceil.x)
  {
    for (
      vNode = &((*hNode)->vList);
      *vNode != NULL && (*vNode)->row < ceil.y;
      vNode = &((*vNode)->next)
    );

    if (*vNode != NULL && (*vNode)->row == ceil.y)
    {
      for (
        objNode = &((*vNode)->objList);
        *objNode != NULL && (*objNode)->obj != this;
        objNode = &((*objNode)->next)
      );

      if (*objNode != NULL && (*objNode)->obj == this)
      {
        ESGE_ObjStaticList *objNodeNext = (*objNode)->next;
        delete *objNode;
        *objNode = objNodeNext;

        if ((*vNode)->objList == NULL)
        {
          ESGE_ObjStaticVNode *vNodeNext = (*vNode)->next;
          delete *vNode;
          *vNode = vNodeNext;

          if ((*hNode)->vList == NULL)
          {
            ESGE_ObjStaticHNode *hNodeNext = (*hNode)->next;
            delete *hNode;
            *hNode = hNodeNext;
          }
        }
      }
    }
  }

  SGLIB_LIST_DELETE(
    ESGE_ObjStatic,
    list,
    this,
    next
  );
}

ESGE_ObjStaticList*
ESGE_ObjStatic::GetObj(SDL_Point pos)
{
  SDL_Point ceil;
  ESGE_ObjStaticHNode *hNode;

  ceil.x = pos.x/ESGE_OBJ_STATIC_H_RES;
  ceil.y = pos.y/ESGE_OBJ_STATIC_V_RES;

  for (
    hNode = hList;
    hNode != NULL && hNode->col < ceil.x;
    hNode = hNode->next
  );

  if (hNode != NULL && hNode->col == ceil.x)
  {
    ESGE_ObjStaticVNode *vNode;

    for (
      vNode = hNode->vList;
      vNode != NULL && vNode->row < ceil.y;
      vNode = vNode->next
    );

    if (vNode != NULL && vNode->row == ceil.y) return vNode->objList;
  }

  return NULL;
}