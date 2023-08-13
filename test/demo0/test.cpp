#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_file.h"
#include "ESGE_spritesheet.h"

#include "ESGE_objEvent.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objStatic.h"
#include "ESGE_objDynamic.h"
#include "ESGE_objDrawSprite.h"

#include "ESGE_objStatic.h"


#define PLAYER_POS_SCALE 4
#define PLAYER_ACC 0x01
#define PLAYER_VEL 0x10
#define PLAYER_GRA 0x01
#define PLAYER_JMP 0x20
#define PLAYER_SS "player.sprite.bin"

#define ROUND(T, S, N) ( \
  ((N + (1<<(S-1)) + (N>>(sizeof(T)*8-1))) & (~((1<<S)-1))) >> S \
)

class ObjPlayer:
  public ESGE_ObjScene,
  public ESGE_ObjUpdate,
  public ESGE_ObjDynamic,
  public ESGE_ObjDrawSprite
{
public:
  SDL_Point fAcc = {0, PLAYER_GRA};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos = {0, 0};
  ESGE_Spritesheet *spritesheet;


  static int GetPosX(void *obj)
  {
    return ((ObjPlayer*)obj)->pos.x;
  }
  static void SetPosX(void *obj, int value)
  {
    ((ObjPlayer*)obj)->pos.x = value;
  }

  static int GetPosY(void *obj)
  {
    return ((ObjPlayer*)obj)->pos.y;
  }
  static void SetPosY(void *obj, int value)
  {
    ((ObjPlayer*)obj)->pos.y = value;
  }


  ObjPlayer(void)
  {
    layer = 1;
    offsetSize.x = 0;
    offsetSize.y = 0;
    offsetSize.w = 16;
    offsetSize.h = 32;
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(PLAYER_SS);
    spritesheet->GetSprite(&sprite, 0, 0);
  }

  virtual ~ObjPlayer(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }

  virtual void OnUpdate(void)
  {
    const Uint8 *keys;

    keys = SDL_GetKeyboardState(NULL);

    fVel.x += fAcc.x;
    fVel.y += fAcc.y;
      
    if (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT])
    {
      if (fVel.x < -PLAYER_VEL)
      {
        fAcc.x = 0;
        fVel.x = -PLAYER_VEL;
      }
      else fAcc.x = -PLAYER_ACC;
    }
    else if (!keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT])
    {
      if (fVel.x > PLAYER_VEL)
      {
        fAcc.x = 0;
        fVel.x = PLAYER_VEL;
      }
      else fAcc.x = PLAYER_ACC;
    }
    else
    {
      if (fVel.x > 0)
        fAcc.x = -PLAYER_ACC;
      else if ((fVel.x < 0))
        fAcc.x = PLAYER_ACC;
      else
      {
        fAcc.x = 0;
        fVel.x = 0;
      }
    }

    if (keys[SDL_SCANCODE_SPACE])
    {
      if (fVel.y >> PLAYER_POS_SCALE == 0)
      {
        SDL_Point test;
        SDL_Rect thisColBox;
        bool onAir = true;

        thisColBox = GetColBox();
        test.x = thisColBox.x;
        test.y = thisColBox.y + thisColBox.h;

        while (test.x < thisColBox.x + thisColBox.w && onAir)
        {
          onAir = ESGE_ObjStatic::GetObjAt(test) == NULL;
          test.x += ESGE_OBJ_STATIC_W;
        }
        if (onAir)
        {
          test.x = thisColBox.x + thisColBox.w - 1;
          onAir = ESGE_ObjStatic::GetObjAt(test) == NULL;
        }
        if (!onAir)
        {
          fVel.y = -PLAYER_JMP;
        }
      }
    }
    else if (fVel.y < 0)
    {
      fVel.y = 0;
    }

    fPos.x += fVel.x;
    fPos.y += fVel.y;

    pos.x = ROUND(int, PLAYER_POS_SCALE, fPos.x);
    pos.y = ROUND(int, PLAYER_POS_SCALE, fPos.y);
  }

  virtual void OnPhysic(void) override
  {
    ESGE_ObjDynamic::OnPhysic();

    ESGE_Display::cam.x = pos.x - 128 + 8;
    ESGE_Display::cam.y = pos.y - 72 + 16;
  }

  virtual void OnCollide(SDL_UNUSED ESGE_ObjCollider *other)
  {
    SDL_Rect thisColBox, otherColBox;

    thisColBox  = GetColBox();
    otherColBox = other->GetColBox();

    if ((
        thisColBox.x++,
        SDL_HasIntersection(&thisColBox, &otherColBox)
      ) || (
        thisColBox.x -= 3,
        SDL_HasIntersection(&thisColBox, &otherColBox)
      )
    )
    {
      fPos.x = pos.x << PLAYER_POS_SCALE;
      fVel.x = 0;
    }
    thisColBox.x = pos.x;
    if ((
        thisColBox.y++,
        SDL_HasIntersection(&thisColBox, &otherColBox)
      ) || (
        thisColBox.y -= 3,
        SDL_HasIntersection(&thisColBox, &otherColBox)
      )
    )
    {
      fPos.y = pos.y << PLAYER_POS_SCALE;
      fVel.y = 0;
    }
  }


  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableUpdate();
    EnableDynamic();
    EnableDraw();
  }

  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableUpdate();
    DisableDynamic();
    DisableDraw();
  }

#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override
  {
    EnableDraw();
  }

  virtual void OnEditorQuit(void) override
  {
    DisableDraw();
  }
#endif
};

ESGE_TYPE(
  ObjPlayer,
  ESGE_FIELD(
    I,
    pos.x,
    ObjPlayer::GetPosX,
    ObjPlayer::SetPosX,
    ESGE_FIELD(
      I,
      pos.y,
      ObjPlayer::GetPosY,
      ObjPlayer::SetPosY,
      ESGE_NO_FIELD
    )
  )
)


#define TILE_SS "tiles.sprite.bin"

class ObjTile:
  public ESGE_ObjScene,
  public ESGE_ObjDrawSprite,
  public ESGE_ObjStatic
{
public:
  ESGE_Spritesheet *spritesheet;
  SDL_Point spriteCell = {0, 0};


  static int GetCellX(void *obj)
  {
    return ((ObjTile*)obj)->pos.x / ESGE_OBJ_STATIC_W;
  }
  static void SetCellX(void *obj, int value)
  {
    ((ObjTile*)obj)->pos.x = value * ESGE_OBJ_STATIC_W;
  }

  static int GetCellY(void *obj)
  {
    return ((ObjTile*)obj)->pos.y / ESGE_OBJ_STATIC_H;
  }
  static void SetCellY(void *obj, int value)
  {
    ((ObjTile*)obj)->pos.y = value * ESGE_OBJ_STATIC_H;
  }

  static int GetSpriteCellX(void *obj)
  {
    return ((ObjTile*)obj)->spriteCell.x;
  }
  static void SetSpriteCellX(void *obj, int value)
  {
    ((ObjTile*)obj)->spriteCell.x = value;
    ((ObjTile*)obj)->spritesheet->GetSprite(
      &((ObjTile*)obj)->sprite,
      ((ObjTile*)obj)->spriteCell.x,
      ((ObjTile*)obj)->spriteCell.y
    );
  }

  static int GetSpriteCellY(void *obj)
  {
    return ((ObjTile*)obj)->spriteCell.y;
  }
  static void SetSpriteCellY(void *obj, int value)
  {
    ((ObjTile*)obj)->spriteCell.y = value;
    ((ObjTile*)obj)->spritesheet->GetSprite(
      &((ObjTile*)obj)->sprite,
      ((ObjTile*)obj)->spriteCell.x,
      ((ObjTile*)obj)->spriteCell.y
    );
  }


  ObjTile(void)
  {
    layer = 1;
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(TILE_SS);
    spritesheet->GetSprite(&sprite, spriteCell.x, spriteCell.y);
  }
  virtual ~ObjTile(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  }


#ifdef ESGE_EDITOR
  virtual void OnEditorInit(void) override
  {
    EnableDraw();
  }

  virtual void OnEditorQuit(void) override
  {
    DisableDraw();
  }
#endif

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

ESGE_TYPE(
  ObjTile,
  ESGE_FIELD(
    I,
    cell.x,
    ObjTile::GetCellX,
    ObjTile::SetCellX,
    ESGE_FIELD(
      I,
      cell.y,
      ObjTile::GetCellY,
      ObjTile::SetCellY,
      ESGE_FIELD(
        I,
        spriteCell.x,
        ObjTile::GetSpriteCellX,
        ObjTile::SetSpriteCellX,
        ESGE_FIELD(
          I,
          spriteCell.y,
          ObjTile::GetSpriteCellY,
          ObjTile::SetSpriteCellY,
          ESGE_NO_FIELD
        )
      )
    )
  )
)
