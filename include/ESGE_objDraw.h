#ifndef ESGE_OBJ_DRAW_H_
# define ESGE_OBJ_DRAW_H_

# include <SDL2/SDL.h>

class ESGE_ObjDraw
{
  friend void ESGE_OnDraw(void *userdata, SDL_Renderer *rend);
  const unsigned layer;
  SDL_Renderer * const rend;
  SDL_bool drawing;
public:
  ESGE_ObjDraw(unsigned layer, SDL_Renderer *rend);
  ~ESGE_ObjDraw(void);
  void PlayDraw(void);
  void StopDraw(void);
  SDL_bool IsDrawing(void) const;
protected:
  virtual void OnDraw(SDL_Renderer *rend) = 0;
};

#endif