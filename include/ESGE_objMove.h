#ifndef ESGE_OBJ_MOVE_H_
# define ESGE_OBJ_MOVE_H_

# include <SDL2/SDL.h>
# include "ESGE_plc.h"
# include "ESGE_objPoint.h"
# include "ESGE_objActive.h"
# include "ESGE_objUpdate.h"

# define ESGE_OBJ_MOVE_PRI ESGE_OBJ_UPDATE_PRI+1

class ESGE_ObjMove:
  public virtual ESGE_ObjPoint,
  public virtual ESGE_ObjActive
{
public:
  SDL_Point vel = {0, 0}, acc = {0, 0};

  ESGE_ObjMove(void);
  virtual ~ESGE_ObjMove(void) override = 0;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;

  virtual void OnMove(void);
};

#endif