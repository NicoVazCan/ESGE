#ifndef ESGE_OBJ_DYNAMIC_H_
# define ESGE_OBJ_DYNAMIC_H_

# include <SDL2/SDL.h>
# include "ESGE_objPhysic.h"
# include "ESGE_objCollider.h"

class ESGE_ObjDynamic: public ESGE_ObjPhysic, public ESGE_ObjCollider
{
  static ESGE_ObjDynamic *list;

  ESGE_ObjDynamic *next;
  bool enabledDynamic = false;
  SDL_Point prevPos = {0, 0};

public:
  ESGE_ObjDynamic(void);
  virtual ~ESGE_ObjDynamic(void) = 0;

  void EnableDynamic(void);
  void DisableDynamic(void);
  bool IsEnabledDynamic(void);

  virtual void OnInside(ESGE_ObjDynamic *other);

  virtual void OnPhysic(void) override;
};

#endif