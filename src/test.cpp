#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_file.h"
#include "ESGE_spritesheet.h"

#include "ESGE_objEvent.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objDynamic.h"
#include "ESGE_objDraw.h"

#include "ESGE_objStatic.h"


class ObjTest: public ESGE_ObjScene, public ESGE_ObjDraw
{
public:
  SDL_Point pos = {0,0};
  char name[6] = {'\0','\0','\0','\0','\0','\0'};

  ObjTest(void)
  {
    layer = 1;
    puts(SDL_FUNCTION);
  }
  virtual ~ObjTest(void) override
  {
    puts(SDL_FUNCTION);
  }

  virtual const ESGE_Field *GetFields(size_t *nFields) override
  {
    static ESGE_Field fields[] = {
      {
        ESGE_Field::I,
        "pos.x",
        NULL,
        0
      },
      {
        ESGE_Field::I,
        "pos.y",
        NULL,
        0
      },
      {
        ESGE_Field::S,
        "name",
        NULL,
        6
      }
    };

    fields[0].data = &pos.x;
    fields[1].data = &pos.y;
    fields[2].data = name;
    *nFields = SDL_arraysize(fields);

    return fields;
  }


  virtual void OnDraw(void) override
  {
    SDL_Rect rect = {pos.x, pos.y, 160, 160};
    ESGE_Display::WorldDrawRect(rect, 255,0,0,255);
  }


  virtual void OnEditorInit(void) override
  {
    EnableDraw();
  }

  virtual void OnEditorQuit(void) override
  {
    DisableDraw();
  }


  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableDraw();
  }

  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableDraw();
  }
};

ESGE_ADD_TYPE(ObjTest)

#define PLAYER_ACC 2
#define PLAYER_VEL 20
#define PLAYER_SS "player.sprite.bin"

class ObjPlayer:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDynamic,
  public ESGE_ObjDraw
{
public:
  SDL_Point vel = {0, 0}, acc = {0, 0};
  ESGE_Spritesheet *spritesheet;

  ObjPlayer(void)
  {
    layer = 1;
    offsetSize.x = 0;
    offsetSize.y = 0;
    offsetSize.w = 160;
    offsetSize.h = 320;
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(PLAYER_SS);
  }

  virtual ~ObjPlayer(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }


  virtual const ESGE_Field* GetFields(size_t *nFields) override
  {
    static ESGE_Field fields[] = {
      {
        ESGE_Field::I,
        "pos.x",
        NULL,
        0
      },
      {
        ESGE_Field::I,
        "pos.y",
        NULL,
        0
      }
    };

    fields[0].data = &pos.x;
    fields[1].data = &pos.y;
    *nFields = SDL_arraysize(fields);

    return fields;
  }

  virtual void OnUpdate(void)
  {
    const Uint8 *keys;

    keys = SDL_GetKeyboardState(NULL);

    vel.x += acc.x;
    vel.y += acc.y;

    if (keys[SDL_SCANCODE_UP] && !keys[SDL_SCANCODE_DOWN])
    {
      if (vel.y < -PLAYER_VEL)
      {
        vel.y = -PLAYER_VEL;
        acc.y = 0;
      }
      else acc.y = -PLAYER_ACC;
    }
    else if (!keys[SDL_SCANCODE_UP] && keys[SDL_SCANCODE_DOWN])
    {
      if (vel.y > PLAYER_VEL)
      {
        vel.y = PLAYER_VEL;
        acc.y = 0;
      }
      else acc.y = PLAYER_ACC;
    }
    else
    {
      if (vel.y == 0)
      {
        vel.y = 0;
        acc.y = 0;
      }
      else if (vel.y > 0) acc.y = -PLAYER_ACC;
      else                acc.y = PLAYER_ACC;
    }
      
    if (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT])
    {
      if (vel.x < -PLAYER_VEL)
      {
        vel.x = -PLAYER_VEL;
        acc.x = 0;
      }
      else acc.x = -PLAYER_ACC;
    }
    else if (!keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT])
    {
      if (vel.x > PLAYER_VEL)
      {
        vel.x = PLAYER_VEL;
        acc.x = 0;
      }
      else acc.x = PLAYER_ACC;
    }
    else
    {
      if (vel.x == 0)
      {
        vel.x = 0;
        acc.x = 0;
      }
      else if (vel.x > 0) acc.x = -PLAYER_ACC;
      else                acc.x = PLAYER_ACC;
    }

    pos.x += vel.x;
    pos.y += vel.y;
  }

  virtual void OnPhysic(void) override
  {
    ESGE_ObjDynamic::OnPhysic();

    ESGE_Display::cam.x = pos.x - 1200;
    ESGE_Display::cam.y = pos.y - 560;
  }

  virtual void OnDraw(void) override
  {
    ESGE_Sprite sprite;

    spritesheet->GetSprite(&sprite, 0, 0);
    ESGE_Display::WorldDrawSprite(&sprite, pos);
  }


  virtual void OnEnable(void)
  {
    ESGE_ObjScene::OnEnable();
    EnableUpdate();
    EnableDynamic();
    EnableDraw();
  }

  virtual void OnDisable(void)
  {
    ESGE_ObjScene::OnDisable();
    DisableUpdate();
    DisableDynamic();
    DisableDraw();
  }


  virtual void OnEditorInit(void) override
  {
    EnableDraw();
  }

  virtual void OnEditorQuit(void) override
  {
    DisableDraw();
  }
};

ESGE_ADD_TYPE(ObjPlayer)


#define TILE_SS "tiles.sprite.bin"

class ObjTile:
  public ESGE_ObjScene,
  public ESGE_ObjDraw,
  public ESGE_ObjStatic
{
public:
  ESGE_Spritesheet *spritesheet;

  ObjTile(void)
  {
    layer = 1;
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(TILE_SS);
  }
  virtual ~ObjTile(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }

  virtual const ESGE_Field *GetFields(size_t *nFields) override
  {
    static ESGE_Field fields[] = {
      {
        ESGE_Field::I,
        "pos.x",
        NULL,
        0
      },
      {
        ESGE_Field::I,
        "pos.y",
        NULL,
        0
      }
    };

    fields[0].data = &pos.x;
    fields[1].data = &pos.y;
    *nFields = SDL_arraysize(fields);

    return fields;
  }


  virtual void OnDraw(void) override
  {
    ESGE_Sprite sprite;

    spritesheet->GetSprite(&sprite, 0, 0);
    ESGE_Display::WorldDrawSprite(&sprite, pos);
  }


  virtual void OnEditorInit(void) override
  {
    EnableDraw();
  }

  virtual void OnEditorQuit(void) override
  {
    DisableDraw();
  }


  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableDraw();
    EnableStatic();
  }

  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableDraw();
    DisableStatic();
  }
};

ESGE_ADD_TYPE(ObjTile)
