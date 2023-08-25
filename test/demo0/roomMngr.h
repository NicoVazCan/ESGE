#ifndef ROOM_MNGR_H_
# define ROOM_MNGR_H_

# include <SDL2/SDL.h>
# include "ESGE_scene.h"

struct Room
{
  const SDL_Rect area;
  const char *const sceneFile;
  bool loaded;
};


class ObjRoomMngr: public ESGE_ObjScene
{
  SDL_Rect focus;
  Room *rooms;
  size_t nRooms;

  inline void LoadRooms(void);

public:
  ObjRoomMngr(void);
  virtual ~ObjRoomMngr(void) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
  
  void SetFocusCenter(int x, int y);
  void CloseRooms(void);
};

#endif