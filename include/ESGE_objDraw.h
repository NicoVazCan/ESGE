#ifndef ESGE_OBJ_DRAW_H_
# define ESGE_OBJ_DRAW_H_

# include <SDL2/SDL.h>
# include "ESGE_objActive.h"
# include "ESGE_objCam.h"
# include "ESGE_objCam.h"

class ESGE_ObjDraw: public virtual ESGE_ObjActive
{
public:
  unsigned layer = 0;

  ESGE_ObjDraw(void);
  virtual ~ESGE_ObjDraw(void);

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;

  virtual void OnDraw(const ESGE_ObjCam *cam) const = 0;
};

#endif