#ifndef ESGE_OBJ_COLLIDER_H_
# define ESGE_OBJ_COLLIDER_H_

# include <SDL2/SDL.h>
# include "ESGE_objBox.h"


class ESGE_ObjCollider: public ESGE_ObjBox
{
public:
  ESGE_ObjCollider(void);
  virtual ~ESGE_ObjCollider(void) = 0;

  virtual void OnCollideUp(ESGE_ObjCollider *other);
  virtual void OnCollideDown(ESGE_ObjCollider *other);
  virtual void OnCollideLeft(ESGE_ObjCollider *other);
  virtual void OnCollideRight(ESGE_ObjCollider *other);
};

#endif
