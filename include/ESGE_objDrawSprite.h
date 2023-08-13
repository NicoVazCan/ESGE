#ifndef ESGE_OBJ_DRAW_SPRITE_H_
# define ESGE_OBJ_DRAW_SPRITE_H_

# include "ESGE_display.h"

# include "ESGE_objPoint.h"
# include "ESGE_objDraw.h"


class ESGE_ObjDrawSprite:
  public virtual ESGE_ObjPoint,
  public ESGE_ObjDraw
{
public:
  ESGE_Sprite sprite;

  ESGE_ObjDrawSprite(void);
  virtual ~ESGE_ObjDrawSprite(void) = 0;
  
  virtual void OnDraw(void) override;
};

#endif