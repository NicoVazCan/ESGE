#ifndef ESGE_OBJ_DISPLAY_H_
# define ESGE_OBJ_DISPLAY_H_

# include <SDL2/SDL.h>


class ESGE_ObjDisplay
{
public:
  ESGE_ObjDisplay(void);
  ~ESGE_ObjDisplay(void);
  SDL_Renderer *rend;
};

#endif