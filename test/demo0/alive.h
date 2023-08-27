#ifndef ALIVE_H_
# define ALIVE_H_

# include <SDL2/SDL.h>
# include "ESGE_objBox.h"

class ObjAlive: public ESGE_ObjBox
{
public:
  int life = 0;
  
  ObjAlive(void);
  virtual ~ObjAlive(void) = 0;

  virtual void OnAttack(int dmg);
};

#endif