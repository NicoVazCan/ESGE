#ifndef ESGE_OBJ_DRAW_H_
# define ESGE_OBJ_DRAW_H_

# include <SDL2/SDL.h>
# include "ESGE_objActive.h"

class ESGE_ObjDraw: public virtual ESGE_ObjActive
{
  friend void ESGE_OnDraw(void *userdata, SDL_Renderer *rend);
  const unsigned layer;
  SDL_Renderer *rend;
public:
  ESGE_ObjDraw(unsigned layer);
  virtual ~ESGE_ObjDraw(void);
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
  void SetRenderer(SDL_Renderer *rend);
protected:
  virtual void OnDraw(SDL_Renderer *rend) = 0;
};

#endif