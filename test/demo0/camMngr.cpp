#include "camMngr.h"

#include "ESGE_display.h"

#define CAM_W 256
#define CAM_H 144


static const SDL_Rect _visibleAreas[] = {
  {0,       0, 256*2, 144},
  {256*2*1, 0, 256*2, 144},
  {256*2*2, 0, 256*2, 144}
};


ObjCamMngr::ObjCamMngr(void):
  visibleAreas(_visibleAreas),
  nVisibleAreas(SDL_arraysize(_visibleAreas))
{}

ObjCamMngr::~ObjCamMngr(void)
{}


void
ObjCamMngr::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  ESGE_ShareObj<ObjCamMngr>(this);
}

void
ObjCamMngr::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  ESGE_UnshareObj<ObjCamMngr>(this);
}


void
ObjCamMngr::SetCamCenter(int x, int y)
{
  SDL_Point center;

  center.x = x;
  center.y = y;

  x -= CAM_W / 2;
  y -= CAM_H / 2;


  for (size_t i = 0; i < nVisibleAreas; ++i)
  {
    if (SDL_PointInRect(&center, visibleAreas+i))
    {
      if (x < visibleAreas[i].x)
        x = visibleAreas[i].x;
      if (y < visibleAreas[i].y)
        y = visibleAreas[i].y;
      if (x + CAM_W > visibleAreas[i].x + visibleAreas[i].w)
        x = visibleAreas[i].x + visibleAreas[i].w - CAM_W;
      if (y + CAM_H > visibleAreas[i].y + visibleAreas[i].h)
        y = visibleAreas[i].y + visibleAreas[i].h - CAM_H;

      break;
    }
  }

  ESGE_Display::cam.x = x;
  ESGE_Display::cam.y = y;
  /*
  SDL_Rect camArea = {x, y, CAM_W, CAM_H};
  ESGE_Display::WorldDrawRect(camArea, 0, 0, 255, 255);
  */
}

ESGE_TYPE(ObjCamMngr)
