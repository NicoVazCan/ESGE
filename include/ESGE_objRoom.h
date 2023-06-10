#ifndef ESGE_OBJ_ROOM_H_
# define ESGE_OBJ_ROOM_H_

# include <SDL2/SDL.h>
# include "ESGE_objScene.h"
# include "ESGE_objCam.h"

# define ESGE_OBJ_ROOM_TYPE_ID 32u
# define ESGE_OBJ_ROOM_FILE_LEN 32

struct ESGE_Room
{
  SDL_Rect area;
  char file[ESGE_OBJ_ROOM_FILE_LEN];
  ESGE_ObjScene *scene;
  ESGE_Room *next;
};

class ESGE_ObjRoomMngr: public ESGE_ObjInScene
{
  friend ESGE_ObjSerial *ESGE_LoadObjRoomMngr(SDL_RWops *io);

  ESGE_Room *enableRooms = NULL;
  ESGE_Room *disabledRooms = NULL;
  ESGE_Room *unloadRooms;
  ESGE_ObjCam *cam = NULL;

  const Uint16 camID;

public:
  ESGE_ObjRoomMngr(Uint16 id, Uint16 camID, ESGE_Room *unloadRooms);
  virtual ~ESGE_ObjRoomMngr(void) override;
  virtual void OnSave(SDL_RWops *io) const override;
  virtual Uint16 GetTypeID(void) const override;
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;
};

#endif