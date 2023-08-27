#ifndef ESGE_OBJ_STATIC_H_
# define ESGE_OBJ_STATIC_H_

# include "ESGE_objCell.h"
# include "ESGE_objCollider.h"


struct ESGE_ListV
{
  unsigned row;
  class ESGE_ObjStatic *obj;
  ESGE_ListV *next;
};

struct ESGE_ListH
{
  unsigned col;
  ESGE_ListV *listV;
  ESGE_ListH *next;
};


class ESGE_ObjStatic: public ESGE_ObjCell, public ESGE_ObjCollider
{
  static ESGE_ListH *listH;

  bool enabledStatic = false;

public:
  static ESGE_ObjStatic *GetObjAt(SDL_Point pos);
  static ESGE_ObjStatic *GetObjAtUp(SDL_Rect box);
  static ESGE_ObjStatic *GetObjAtDown(SDL_Rect box);
  static ESGE_ObjStatic *GetObjAtLeft(SDL_Rect box);
  static ESGE_ObjStatic *GetObjAtRight(SDL_Rect box);

  ESGE_ObjStatic(void);
  virtual ~ESGE_ObjStatic(void) = 0;

  int GetColW(void);
  void SetColW(int colW);

  int GetRowH(void);
  void SetRowH(int rowH);

  void EnableStatic(void);
  void DisableStatic(void);
  bool IsEnabledStatic(void);
};

#endif