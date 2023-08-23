#ifndef ALIVE_H_
# define ALIVE_H_

# include <SDL2/SDL.h>
# include "ESGE_objPoint.h"

class ObjAlive: public virtual ESGE_ObjPoint
{
public:
  SDL_Rect offsetSize = {0, 0, 0, 0};
  int life = 0;
  
  ObjAlive(void);
  virtual ~ObjAlive(void) = 0;

  SDL_Rect GetHitBox(void);
  void SetHitBox(SDL_Rect hitBox);

  virtual void OnAttack(int dmg);
};

#endif