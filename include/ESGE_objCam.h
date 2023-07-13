#ifndef ESGE_OBJ_CAM_H_
# define ESGE_OBJ_CAM_H_

# include <SDL2/SDL.h>
# include "ESGE_scene.h"
# include "ESGE_objPoint.h"

# define ESGE_OBJ_CAM_WORLD_FACTOR 10

class ESGE_ObjCam: public ESGE_ObjScene, public ESGE_ObjPoint
{
public:
  static ESGE_ObjCam *list;

  SDL_FORCE_INLINE int PixelToWorld(int pixel)
  {
    return pixel*ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  SDL_FORCE_INLINE int SubpixelToWorld(float subpixel)
  {
    return SDL_lroundf(subpixel)*ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  
  SDL_FORCE_INLINE int WorldToPixel(int pixel)
  {
    return pixel/ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  SDL_FORCE_INLINE float WorldToSubpixel(int world)
  {
    return ((float)world)/((float)ESGE_OBJ_CAM_WORLD_FACTOR);
  }

  SDL_Renderer *render = NULL;
  ESGE_ObjCam *next;

  ESGE_ObjCam(void);
  virtual ~ESGE_ObjCam(void) = 0;

  SDL_Point WorldToDisplayPoint(SDL_Point pos) const;
  SDL_Point DisplayToWorldPoint(SDL_Point pos) const;
};

#endif