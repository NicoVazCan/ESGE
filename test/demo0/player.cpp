#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_spritesheet.h"
#include "ESGE_anim.h"
#include "ESGE_time.h"
#include "ESGE_audio.h"
#include "ESGE_file.h"

#include "ESGE_objStatic.h"
#include "ESGE_objDynamic.h"

#include "ESGE_objUpdate.h"
#include "ESGE_objDrawSprite.h"


static const ESGE_Frm frmsStandR[] = {
  {0, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {2, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 0, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50}
};

static const ESGE_Frm frmsStandL[] = {
  {0, 1, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 1, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {2, 1, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 1, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50}
};


static const ESGE_Frm frmsStandUR[] = {
  {0, 2, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 2, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {2, 2, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 2, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50}
};

static const ESGE_Frm frmsStandUL[] = {
  {0, 3, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 3, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {2, 3, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 3, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50}
};


static const ESGE_Frm frmsRunR[] = {
  {0, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {2, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {3, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {4, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {5, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {6, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {7, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {8, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {9, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10}
};

static const ESGE_Frm frmsRunL[] = {
  {0, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {2, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {3, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {4, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {5, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {6, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {7, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {8, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {9, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10}
};


static const ESGE_Frm frmsJmpR[] = {
  {0, 6, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 6, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {2, 6, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};

static const ESGE_Frm frmsJmpL[] = {
  {0, 7, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 7, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {2, 7, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};


static const ESGE_Frm frmsJmpUR[] = {
  {0, 8, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 8, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {2, 8, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};

static const ESGE_Frm frmsJmpUL[] = {
  {0, 9, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 9, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {2, 9, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};


static const ESGE_Frm frmsFallR[] = {
  {0, 10, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 10, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};

static const ESGE_Frm frmsFallL[] = {
  {0, 11, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 11, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};


static const ESGE_Frm frmsFallUR[] = {
  {0, 12, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 12, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};

static const ESGE_Frm frmsFallUL[] = {
  {0, 13, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
  {1, 13, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*10},
};


static const ESGE_Frm frmsDmgR[] = {
  {0, 14, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 14, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
};

static const ESGE_Frm frmsDmgL[] = {
  {0, 15, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
  {1, 15, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*50},
};


static const ESGE_Frm frmsBall[] = {
  {0, 16, 1., 0., {0, 0}, SDL_FLIP_NONE, 0},
};


static const ESGE_Frm frmsBallR[] = {
  {0, 16, 1., 45.,  {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 90.,  {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 135., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 180., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 225., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 270., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 315., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., 360., {20, 36}, SDL_FLIP_NONE, 16*10},
};

static const ESGE_Frm frmsBallL[] = {
  {0, 16, 1., -45.,  {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -90.,  {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -135., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -180., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -225., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -270., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -315., {20, 36}, SDL_FLIP_NONE, 16*10},
  {0, 16, 1., -360., {20, 36}, SDL_FLIP_NONE, 16*10},
};


static const ESGE_Anim anims[] = {
  {frmsStandR, SDL_arraysize(frmsStandR), -1},
  {frmsStandL, SDL_arraysize(frmsStandL), -1},

  {frmsStandUR, SDL_arraysize(frmsStandUR), -1},
  {frmsStandUR, SDL_arraysize(frmsStandUR), -1},

  {frmsRunR, SDL_arraysize(frmsRunR), -1},
  {frmsRunL, SDL_arraysize(frmsRunL), -1},

  {frmsJmpR, SDL_arraysize(frmsJmpR), 0},
  {frmsJmpL, SDL_arraysize(frmsJmpL), 0},

  {frmsJmpUR, SDL_arraysize(frmsJmpUR), 0},
  {frmsJmpUL, SDL_arraysize(frmsJmpUL), 0},

  {frmsFallR, SDL_arraysize(frmsFallR), 0},
  {frmsFallL, SDL_arraysize(frmsFallL), 0},

  {frmsFallUR, SDL_arraysize(frmsFallUR), 0},
  {frmsFallUL, SDL_arraysize(frmsFallUL), 0},

  {frmsDmgR, SDL_arraysize(frmsDmgR), 0},
  {frmsDmgL, SDL_arraysize(frmsDmgL), 0},

  {frmsBall, SDL_arraysize(frmsBall), 0},

  {frmsBallR, SDL_arraysize(frmsBallR), -1},
  {frmsBallL, SDL_arraysize(frmsBallL), -1},
};



enum PlayerAnim: size_t
{
  STAND_R, STAND_L,
  STAND_U_R, STAND_U_L,
  RUN_R, RUN_L,
  JMP_R, JMP_L,
  JMP_U_R, JMP_U_L,
  FALL_R, FALL_L,
  FALL_U_R, FALL_U_L,
  DMG_R, DMG_L,
  BALL,
  BALL_R, BALL_L
};


#define PLAYER_POS_SCALE 8
#define PLAYER_ACC ( \
  ((int)(ESGE_deltaTm * ESGE_deltaTm)) * 0x0010 / 256 \
)
#define PLAYER_VEL (((int)ESGE_deltaTm) * 0x0100 / 16)
#define PLAYER_GRA ( \
  ((int)(ESGE_deltaTm * ESGE_deltaTm)) * 0x0010 / 256 \
)
#define PLAYER_JMP (((int)ESGE_deltaTm) * 0x0200 / 16)
#define PLAYER_JMP_SND "jump.wav"
#define PLAYER_SS "player.sprite.bin"
#define PLAYER_MUS "brinstar.wav"

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
  SDL_Point fAcc = {0, 0};
  SDL_Point fVel = {0, 0};
  SDL_Point fPos;
  ESGE_Spritesheet *spritesheet;
  ESGE_AnimPlayer animPlayer;
  ESGE_Music *music;
  ESGE_Sound *jmpSnd;


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
    offsetSize.x = 0;
    offsetSize.y = 0;
    offsetSize.w = 16;
    offsetSize.h = 32;

    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(PLAYER_SS);

    animPlayer.sprts = spritesheet;
    animPlayer.speed = 100;

    animPlayer.Start(anims + RUN_R);
    animPlayer.GetSprite(&sprite);

    music = ESGE_FileMngr<ESGE_Music>::Watch(PLAYER_MUS);
    jmpSnd = ESGE_FileMngr<ESGE_Sound>::Watch(PLAYER_JMP_SND);
  }

  virtual ~ObjPlayer(void) override
  {
    ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
    ESGE_FileMngr<ESGE_Music>::Leave(music);
    ESGE_FileMngr<ESGE_Sound>::Leave(jmpSnd);
  }


  virtual void OnInit(void)
  {
    fPos.x = pos.x << PLAYER_POS_SCALE;
    fPos.y = pos.y << PLAYER_POS_SCALE;
    music->Play();
  }



  virtual void OnUpdate(void)
  {
    const Uint8 *keys;

    if (fVel.y + fAcc.y >= PLAYER_JMP)
    {
      fAcc.y = 0;
      fVel.y = PLAYER_JMP;
    }
    else fAcc.y = PLAYER_GRA;

    keys = SDL_GetKeyboardState(NULL);
      
    if (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT])
    {
      if (fVel.x - PLAYER_ACC < -PLAYER_VEL)
      {
        fAcc.x = 0;
        fVel.x = -PLAYER_VEL;
      }
      else fAcc.x = -PLAYER_ACC;
    }
    else if (!keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT])
    {
      if (fVel.x + PLAYER_ACC > PLAYER_VEL)
      {
        fAcc.x = 0;
        fVel.x = PLAYER_VEL;
      }
      else fAcc.x = PLAYER_ACC;
    }
    else
    {
      if (fVel.x > 0)
      {
        if (fVel.x + fAcc.x <= 0)
        {
          fAcc.x = 0;
          fVel.x = 0;
        }
        else fAcc.x = -PLAYER_ACC;
      }
      else if ((fVel.x < 0))
      {
        if (fVel.x + fAcc.x >= 0)
        {
          fAcc.x = 0;
          fVel.x = 0;
        }
        else fAcc.x = PLAYER_ACC;
      }
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
          test.x += ESGE_ObjStatic::cellW;
        }
        if (onAir)
        {
          test.x = thisColBox.x + thisColBox.w - 1;
          onAir = ESGE_ObjStatic::GetObjAt(test) == NULL;
        }
        if (!onAir)
        {
          fVel.y = -PLAYER_JMP;
          jmpSnd->Play();
        }
      }
    }
    else if (fVel.y < 0)
    {
      fVel.y = 0;
    }

    fPos.x += fVel.x += fAcc.x;
    fPos.y += fVel.y += fAcc.y;

    pos.x = ROUND(int, PLAYER_POS_SCALE, fPos.x);
    pos.y = ROUND(int, PLAYER_POS_SCALE, fPos.y);

    animPlayer.Update(ESGE_deltaTm);
    animPlayer.GetSprite(&sprite);
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
        thisColBox.x -= 2,
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
        thisColBox.y -= 2,
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
