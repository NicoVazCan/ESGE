#ifndef ESGE_OBJ_CAM_H_
# define ESGE_OBJ_CAM_H_

# include <SDL2/SDL.h>
# include "ESGE_objSerial.h"
# include "ESGE_objScene.h"
# include "ESGE_objSceneEditor.h"
# include "ESGE_objPoint.h"

# include <stdio.h>

# define ESGE_OBJ_CAM_TYPE_ID 3

# define ESGE_OBJ_CAM_WORLD_FACTOR 10
# define ESGE_OBJ_CAM_CENTER_H_OFFSET ESGE_OBJ_CAM_WORLD_FACTOR*256/2
# define ESGE_OBJ_CAM_CENTER_V_OFFSET ESGE_OBJ_CAM_WORLD_FACTOR*144/2

class ESGE_ObjCam: public ESGE_ObjInScene, public ESGE_ObjPoint
{
public:
  static ESGE_ObjCam *list;
  ESGE_ObjCam *next;

  ESGE_ObjCam(Uint16 id, SDL_Point pos);
  virtual ~ESGE_ObjCam(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;

  void SetCenter(SDL_Point pos);

  SDL_Point WorldToDisplayPoint(SDL_Point pos) const;
  SDL_Point DisplayToWorldPoint(SDL_Point pos) const;

  static inline int PixelToWorld(int pixel)
  {
    return pixel*ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  static inline int SubpixelToWorld(float subpixel)
  {
    return SDL_lroundf(subpixel)*ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  
  static inline int WorldToPixel(int pixel)
  {
    return pixel/ESGE_OBJ_CAM_WORLD_FACTOR;
  }
  static inline float WorldToSubpixel(int world)
  {
    return ((float)world)/((float)ESGE_OBJ_CAM_WORLD_FACTOR);
  }
};

#endif