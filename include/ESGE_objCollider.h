#ifndef ESGE_OBJ_COLLIDER_H_
# define ESGE_OBJ_COLLIDER_H_

# include <SDL2/SDL.h>
# include "ESGE_objPoint.h"


class ESGE_ObjCollider: public virtual ESGE_ObjPoint
{
public:
  SDL_Rect offsetSize = {0, 0, 0, 0};

  ESGE_ObjCollider(void);
  virtual ~ESGE_ObjCollider(void) = 0;

  SDL_Rect GetColBox(void);
  void SetColBox(SDL_Rect colBox);

  virtual void OnCollide(ESGE_ObjCollider *other);
};

#endif
