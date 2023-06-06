#ifndef ESGE_OBJ_POINT_H_
# define ESGE_OBJ_POINT_H_

# include <SDL2/SDL.h>

class ESGE_ObjPoint
{
public:
  static ESGE_ObjPoint *list;
  ESGE_ObjPoint *next;
  SDL_Point pos;

  ESGE_ObjPoint(SDL_Point pos);
  virtual ~ESGE_ObjPoint(void);
};

#endif