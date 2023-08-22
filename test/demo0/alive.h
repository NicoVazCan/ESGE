#ifndef ALIVE_H_
# define ALIVE_H_

# include "ESGE_objDynamic.h"


class ObjAlive: public ESGE_ObjDynamic
{
  int life = 0;

public:
  ObjAlive(void);
  virtual ~ObjAlive(void) = 0;

  virtual void Attack(int dmg);
};

#endif