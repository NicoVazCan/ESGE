#ifndef ESGE_OBJ_DYNAMIC_H_
# define ESGE_OBJ_DYNAMIC_H_

# include <SDL2/SDL.h>
# include "ESGE_objMove.h"
# include "ESGE_objCollider.h"
# include "ESGE_objStatic.h"

class ESGE_ObjDynamic:
  public ESGE_ObjMove,
  public ESGE_ObjCollider
{
public:
  static ESGE_ObjDynamic *list;
  ESGE_ObjDynamic *next;

  ESGE_ObjDynamic(
    SDL_Point pos,
    SDL_Point vel,
    SDL_Point acc,
    SDL_Rect offsetSize
  );
  virtual ~ESGE_ObjDynamic(void) = 0;

protected:
  virtual void OnMove(void) override;
};

#endif