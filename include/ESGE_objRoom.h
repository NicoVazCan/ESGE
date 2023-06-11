#ifndef ESGE_OBJ_ROOM_H_
# define ESGE_OBJ_ROOM_H_

# include <SDL2/SDL.h>
# include "ESGE_objScene.h"
# include "ESGE_objCam.h"

# define ESGE_OBJ_ROOM_MNGR_TYPE_ID 32
# define ESGE_OBJ_ROOM_MNGR_UPDATE_PRI 1

struct ESGE_Room
{
  SDL_Rect area;
  char file[ESGE_OBJ_SCENE_FILE_LEN];
  bool enable;
  ESGE_ObjScene *scene;
};

class ESGE_ObjRoomMngr: public ESGE_ObjInScene
{
  friend ESGE_ObjSerial *ESGE_LoadObjRoomMngr(SDL_RWops *io);
  friend void ESGE_ObjRoomMngrUpdate(void *userdata);

  ESGE_ObjScene *enableRooms = NULL;
  ESGE_ObjScene *disabledRooms = NULL;
  ESGE_ObjScene *unloadRooms = NULL;
  ESGE_ObjCam *cam = NULL;

  const Uint16 camID;
  const Uint16 nRooms;
  ESGE_Room *const rooms;

public:
  ESGE_ObjRoomMngr(
    Uint16 id,
    Uint16 camID,
    Uint16 nRooms,
    ESGE_Room *rooms
  );
  virtual ~ESGE_ObjRoomMngr(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
};

#endif