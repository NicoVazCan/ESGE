#ifndef ESGE_OBJ_STATIC_H_
# define ESGE_OBJ_STATIC_H_

# include <SDL2/SDL.h>
# include "ESGE_objCam.h"
# include "ESGE_objCollider.h"

# define ESGE_OBJ_STATIC_H_RES 160
# define ESGE_OBJ_STATIC_V_RES 160

struct ESGE_ObjStaticHNode;
struct ESGE_ObjStaticVNode;

class ESGE_ObjStatic: public ESGE_ObjCollider
{
  static ESGE_ObjStaticHNode *hList;

public:
  ESGE_ObjStatic *next;

  ESGE_ObjStatic(SDL_Point pos, SDL_Rect offsetSize);
  virtual ~ESGE_ObjStatic(void) = 0;
  static ESGE_ObjStatic *GetObjsAt(SDL_Point pos);
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
  ESGE_ObjStatic *objList;
  ESGE_ObjStaticVNode *next;
};

#endif