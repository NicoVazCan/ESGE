#ifndef CAM_MNGR_H_
# define CAM_MNGR_H_

# include <SDL2/SDL.h>
# include "ESGE_scene.h"


class ObjCamMngr: public ESGE_ObjScene
{
  const SDL_Rect *visibleAreas;
  size_t nVisibleAreas;

public:
  ObjCamMngr(void);
  virtual ~ObjCamMngr(void) override;
  
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
  
  void SetCamCenter(int x, int y);
};

#endif