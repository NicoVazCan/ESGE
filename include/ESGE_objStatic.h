#ifndef ESGE_OBJ_STATIC_H_
# define ESGE_OBJ_STATIC_H_

# include <SDL2/SDL.h>
# include "ESGE_objCam.h"
# include "ESGE_objCollider.h"

# define ESGE_OBJ_STATIC_H_RES ESGE_ObjCam::PixelToWorld(16)
# define ESGE_OBJ_STATIC_V_RES ESGE_ObjCam::PixelToWorld(16)

struct ESGE_ObjStaticHNode;
struct ESGE_ObjStaticVNode;
struct ESGE_ObjStaticList;

class ESGE_ObjStatic: public ESGE_ObjCollider
{
  ESGE_ObjStaticHNode *hList;

public:
  static ESGE_ObjStatic *list;
  ESGE_ObjStatic *next;

  ESGE_ObjStatic(SDL_Point pos, SDL_Rect colBox);
  virtual ~ESGE_ObjStatic(void) = 0;
  ESGE_ObjStaticList *GetObj(SDL_Point pos);
};

struct ESGE_ObjStaticHNode
{
  int col;
  ESGE_ObjStaticVNode *vList;
  ESGE_ObjStaticHNode *next;
};

struct ESGE_ObjStaticVNode
{
  int row;
  ESGE_ObjStaticList *objList;
  ESGE_ObjStaticVNode *next;
};

struct ESGE_ObjStaticList
{
  ESGE_ObjStatic *obj;
  ESGE_ObjStaticList *next;
};

#endif