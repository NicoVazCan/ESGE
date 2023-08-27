#ifndef ESGE_OBJ_BOX_H_
# define ESGE_OBJ_BOX_H_

# include <SDL2/SDL.h>
# include "ESGE_objPoint.h"

class ESGE_ObjBox: public virtual ESGE_ObjPoint
{
public:
  SDL_Rect offsetSize = {0, 0, 0, 0};
  
  ESGE_ObjBox(void);
  virtual ~ESGE_ObjBox(void) = 0;

  inline SDL_Rect GetBox(void)
  {
    SDL_Rect box = offsetSize;

    box.x += pos.x;
    box.y += pos.y;

    return box;
  }
  inline void SetBox(SDL_Rect box)
  {
    pos.x = box.x - offsetSize.x;
    pos.y = box.y - offsetSize.y;
  }
};

#endif