#ifndef ESGE_OBJ_ROOM_H_
# define ESGE_OBJ_ROOM_H_

# include <SDL2/SDL.h>
# include "ESGE_objScene.h"
# include "ESGE_objSceneEditor.h"

# define ESGE_OBJ_ROOM_MNGR_TYPE_ID 2

struct ESGE_Room
{
  SDL_Rect area;
  char file[ESGE_OBJ_SCENE_FILE_LEN];
  bool enable;
  ESGE_ObjScene *scene;
};

class ESGE_ObjRoomMngr: public ESGE_ObjInScene
{
  const Uint16 nRooms;
  ESGE_Room *const rooms;

public:
  static ESGE_ObjRoomMngr *list;
  ESGE_ObjRoomMngr *next;
  
  ESGE_ObjRoomMngr(
    Uint16 id,
    Uint16 nRooms,
    ESGE_Room *rooms
  );
  virtual ~ESGE_ObjRoomMngr(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
  void SetCenter(SDL_Point center);
};

#endif