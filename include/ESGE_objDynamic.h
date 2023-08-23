#ifndef ESGE_OBJ_DYNAMIC_H_
# define ESGE_OBJ_DYNAMIC_H_

# include <SDL2/SDL.h>
# include "ESGE_objPhysic.h"
# include "ESGE_objCollider.h"

class ESGE_ObjDynamic: public ESGE_ObjPhysic, public ESGE_ObjCollider
{
public:
  SDL_Point prevPos = {0, 0};
  
  ESGE_ObjDynamic(void);
  virtual ~ESGE_ObjDynamic(void) = 0;

  virtual void OnPhysic(void) override;
};

#endif