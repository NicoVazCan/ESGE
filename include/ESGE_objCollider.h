#ifndef ESGE_OBJ_COLLIDER_H_
# define ESGE_OBJ_COLLIDER_H_

# include <SDL2/SDL.h>
# include "ESGE_objPoint.h"

class ESGE_ObjCollider: public virtual ESGE_ObjPoint
{
  const SDL_Rect colBox;

public:
  ESGE_ObjCollider(SDL_Point pos, SDL_Rect colBox);
  virtual ~ESGE_ObjCollider(void);
  SDL_Rect GetColBox(void);
  virtual void OnCollide(ESGE_ObjCollider *other) = 0;
};

#endif