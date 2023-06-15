#include "ESGE_objStatic.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

ESGE_ObjStaticHNode *ESGE_ObjStatic::hList = NULL;

ESGE_ObjStatic::ESGE_ObjStatic(SDL_Point pos, SDL_Rect offsetSize):
  ESGE_ObjPoint(pos),
  ESGE_ObjCollider(pos, offsetSize)
{
  SDL_Point ceil;
  ESGE_ObjStaticHNode **hNode;
  ESGE_ObjStaticVNode **vNode;

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

  if (*vNode == NULL || (*vNode)->row != ceil.y)
  {
    *vNode = new ESGE_ObjStaticVNode{ceil.y, NULL, *vNode};
  }

  SGLIB_LIST_ADD(
    ESGE_ObjStatic,
    (*vNode)->objList,
    this,
    next
  );
}

ESGE_ObjStatic::~ESGE_ObjStatic(void)
{
  SDL_Point ceil;
  ESGE_ObjStaticHNode **hNode;
  ESGE_ObjStaticVNode **vNode;
  ESGE_ObjStatic **objNode;

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
        *objNode != NULL && *objNode != this;
        objNode = &((*objNode)->next)
      );

      SDL_assert(*objNode == this);

      *objNode = (*objNode)->next;

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

ESGE_ObjStatic*
ESGE_ObjStatic::GetObjsAt(SDL_Point pos)
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