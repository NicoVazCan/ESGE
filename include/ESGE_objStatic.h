#ifndef ESGE_OBJ_STATIC_H_
# define ESGE_OBJ_STATIC_H_

# include "ESGE_objCollider.h"

# define ESGE_OBJ_STATIC_W 16
# define ESGE_OBJ_STATIC_H 16


class ESGE_ObjStatic: public ESGE_ObjCollider
{
  static struct ESGE_ObjStaticList
  {
    ESGE_ObjStatic *vList;
    ESGE_ObjStaticList *next;
  } *hList;

  ESGE_ObjStatic *next;
  bool enabledStatic = false;

public:
  static ESGE_ObjStatic *GetObjAt(SDL_Point pos);

  ESGE_ObjStatic(void);
  virtual ~ESGE_ObjStatic(void) = 0;

  void EnableStatic(void);
  void DisableStatic(void);
  bool IsEnabledStatic(void);
};

#endif