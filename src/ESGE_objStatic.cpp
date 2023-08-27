#include "ESGE_objStatic.h"

#include <SDL2/SDL.h>
#include "ESGE_error.h"


ESGE_ListH *ESGE_ObjStatic::listH = NULL;


ESGE_ObjStatic*
ESGE_ObjStatic::GetObjAt(SDL_Point pos)
{
  unsigned col, row;
  ESGE_ListH *nodeH;

  col = ((unsigned)pos.x)/cellW;
  row = ((unsigned)pos.y)/cellH;

  for (
    nodeH = listH;
    nodeH && nodeH->col < col;
    nodeH = nodeH->next
  );

  if (nodeH && nodeH->col == col)
  {
    ESGE_ListV *nodeV;

    for (
      nodeV = nodeH->listV;
      nodeV && nodeV->row < row;
      nodeV = nodeV->next
    );

    if (nodeV && nodeV->row == row)
      return nodeV->obj;
  }
  return NULL;
}

ESGE_ObjStatic*
ESGE_ObjStatic::GetObjAtUp(SDL_Rect box)
{
  SDL_Point test;
  int right;
  ESGE_ObjStatic *obj = NULL;

  test.x = box.x;
  test.y = box.y;
  right = box.x + box.w;

  while (
    test.x < right &&
    !(obj = ESGE_ObjStatic::GetObjAt(test))
  )
    test.x += ESGE_ObjStatic::cellW;

  if (!obj)
  {
    test.x = right - 1;

    obj = ESGE_ObjStatic::GetObjAt(test);
  }

  return obj;
}
ESGE_ObjStatic*
ESGE_ObjStatic::GetObjAtDown(SDL_Rect box)
{
  SDL_Point test;
  int right;
  ESGE_ObjStatic *obj = NULL;

  test.x = box.x;
  test.y = box.y + box.h - 1;
  right = box.x + box.w;

  while (
    test.x < right &&
    !(obj = ESGE_ObjStatic::GetObjAt(test))
  )
    test.x += ESGE_ObjStatic::cellW;
  
  if (!obj)
  {
    test.x = right - 1;
    
    obj = ESGE_ObjStatic::GetObjAt(test);
  }

  return obj;
}
ESGE_ObjStatic*
ESGE_ObjStatic::GetObjAtLeft(SDL_Rect box)
{
  SDL_Point test;
  int botton;
  ESGE_ObjStatic *obj = NULL;

  test.x = box.x;
  test.y = box.y;
  botton = box.y + box.h;

  while (
    test.y < botton &&
    !(obj = ESGE_ObjStatic::GetObjAt(test))
  )
    test.y += ESGE_ObjStatic::cellH;
  
  if (!obj)
  {
    test.y = botton - 1;

    obj = ESGE_ObjStatic::GetObjAt(test);
  }

  return obj;
}
ESGE_ObjStatic*
ESGE_ObjStatic::GetObjAtRight(SDL_Rect box)
{
  SDL_Point test;
  int botton;
  ESGE_ObjStatic *obj = NULL;

  test.x = box.x + box.w - 1;
  test.y = box.y;
  botton = box.y + box.h;

  while (
    test.y < botton &&
    !(obj = ESGE_ObjStatic::GetObjAt(test))
  )
    test.y += ESGE_ObjStatic::cellH;
  
  if (!obj)
  {
    test.y = botton - 1;

    obj = ESGE_ObjStatic::GetObjAt(test);
  }

  return obj;
}


ESGE_ObjStatic::ESGE_ObjStatic(void)
{
  offsetSize.x = 0;
  offsetSize.y = 0;
}

ESGE_ObjStatic::~ESGE_ObjStatic(void)
{}


int
ESGE_ObjStatic::GetColW(void)
{
  return offsetSize.w / cellW;
}

void
ESGE_ObjStatic::SetColW(int colW)
{
  offsetSize.w = colW * cellW;
}


int
ESGE_ObjStatic::GetRowH(void)
{
  return offsetSize.h / cellH;
}

void
ESGE_ObjStatic::SetRowH(int rowH)
{
  offsetSize.h = rowH * cellH;
}


#define ESGE_OBJ_STATIC_ERROR() \
ESGE_Error( \
  "Failed to enable static object: Two objects in %d column and %d " \
  "row", \
  col, \
  row \
)

void
ESGE_ObjStatic::EnableStatic(void)
{
  unsigned col, row, colW, rowH;
  ESGE_ListH **nodeH;
  ESGE_ListV **nodeV;

  enabledStatic = true;

  colW = GetColW();
  rowH = GetRowH();


  col = GetCol();

  for (
    nodeH = &listH;
    *nodeH && (*nodeH)->col < col;
    nodeH = &(*nodeH)->next
  );

  if (!*nodeH || (*nodeH)->col != col)
    *nodeH = new ESGE_ListH{col, NULL, *nodeH};


  row = GetRow();

  for (
    nodeV = &(*nodeH)->listV;
    *nodeV && (*nodeV)->row < row;
    nodeV = &(*nodeV)->next
  );

  if (*nodeV && (*nodeV)->row == row)
    ESGE_OBJ_STATIC_ERROR();

  *nodeV = new ESGE_ListV{row, this, *nodeV};

  for (unsigned i = 1; i < rowH; ++i)
  {
    row++;
    nodeV = &(*nodeV)->next;

    if (*nodeV && (*nodeV)->row == row)
      ESGE_OBJ_STATIC_ERROR();

    *nodeV = new ESGE_ListV{row, this, *nodeV};
  }



  for (unsigned j = 1; j < colW; ++j)
  {
    col++;
    nodeH = &(*nodeH)->next;

    if (!*nodeH || (*nodeH)->col != col)
      *nodeH = new ESGE_ListH{col, NULL, *nodeH};


    row = GetRow();

    for (
      nodeV = &(*nodeH)->listV;
      *nodeV && (*nodeV)->row < row;
      nodeV = &(*nodeV)->next
    );

    if (*nodeV && (*nodeV)->row == row)
      ESGE_OBJ_STATIC_ERROR();

    *nodeV = new ESGE_ListV{row, this, *nodeV};

    for (unsigned i = 1; i < rowH; ++i)
    {
      row++;
      nodeV = &(*nodeV)->next;

      if (*nodeV && (*nodeV)->row == row)
        ESGE_OBJ_STATIC_ERROR();

      *nodeV = new ESGE_ListV{row, this, *nodeV};
    }
  }
}

void
ESGE_ObjStatic::DisableStatic(void)
{
  unsigned colW, rowH;

  enabledStatic = false;

  colW = GetColW();
  rowH = GetRowH();

  for (unsigned j = 0; j < colW; ++j)
  {
    ESGE_ListH **nodeH;
    unsigned col;

    col = GetCol() + j;

    for (
      nodeH = &listH;
      *nodeH && (*nodeH)->col < col;
      nodeH = &(*nodeH)->next
    );

    if (*nodeH && (*nodeH)->col == col)
    {
      for (unsigned i = 0; i < rowH; ++i)
      {
        ESGE_ListV **nodeV;
        unsigned row;

        row = GetRow() + i;

        for (
          nodeV = &(*nodeH)->listV;
          *nodeV && (*nodeV)->row < row;
          nodeV = &(*nodeV)->next
        );

        if (*nodeV && (*nodeV)->row == row)
        {
          {
            ESGE_ListV *next;

            next = (*nodeV)->next;
            delete *nodeV;
            *nodeV = next;
          }
          if (!(*nodeH)->listV)
          {
            ESGE_ListH *next;

            next = (*nodeH)->next;
            delete *nodeH;
            *nodeH = next;
          }
        }
      }
    }
  }
}

bool
ESGE_ObjStatic::IsEnabledStatic(void)
{
  return enabledStatic;
}
