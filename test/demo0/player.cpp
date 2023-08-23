#include "player.h"

#include "ESGE_spritesheet.h"
#include "ESGE_time.h"
#include "ESGE_audio.h"
#include "ESGE_file.h"
#include "ESGE_objStatic.h"
#include "ESGE_objDynamic.h"

#include "roomMngr.h"
#include "camMngr.h"


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
  {0, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {1, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {2, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {3, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {4, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {5, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {6, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {7, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {8, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {9, 4, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6}
};

static const ESGE_Frm frmsRunL[] = {
  {0, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {1, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {2, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {3, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {4, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {5, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {6, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {7, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {8, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6},
  {9, 5, 1., 0., {0, 0}, SDL_FLIP_NONE, 16*6}
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


#define POS_SCALE 8

int
ObjPlayer::GetPosX(void *obj)
{
  return ((ObjPlayer*)obj)->pos.x;
}
void
ObjPlayer::SetPosX(void *obj, int value)
{
  ((ObjPlayer*)obj)->pos.x = value;
  ((ObjPlayer*)obj)->prevPos.x = value;
  ((ObjPlayer*)obj)->fPos.x = value << POS_SCALE;
}

int
ObjPlayer::GetPosY(void *obj)
{
  return ((ObjPlayer*)obj)->pos.y;
}
void
ObjPlayer::SetPosY(void *obj, int value)
{
  ((ObjPlayer*)obj)->pos.y = value;
  ((ObjPlayer*)obj)->prevPos.y = value;
  ((ObjPlayer*)obj)->fPos.y = value << POS_SCALE;
}

#define JMP_SND "sounds/jump.wav"
#define SS "sprites/player.sprite.bin"

ObjPlayer::ObjPlayer(void)
{
  layer = PLAYER_LAYER;
  
  offsetSize.x = 0;
  offsetSize.y = 0;
  offsetSize.w = 16;
  offsetSize.h = 32;

  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(SS);

  animPlayer.sprts = spritesheet;
  animPlayer.speed = 100;

  facingR = true;
  animPlayer.Start(anims + STAND_R);
  animPlayer.GetSprite(&sprite);

  jmpSnd = ESGE_FileMngr<ESGE_Sound>::Watch(JMP_SND);
}

ObjPlayer::~ObjPlayer(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
  ESGE_FileMngr<ESGE_Sound>::Leave(jmpSnd);
}

void
ObjPlayer::OnStart(void)
{
  ObjRoomMngr *roomMngr;
  ObjCamMngr *camMngr;

  if ((roomMngr = ESGE_GetObj<ObjRoomMngr>(sceneID, "ObjRoomMngr")))
    roomMngr->SetFocusCenter(pos.x + 8, pos.y + 16);

  if ((camMngr = ESGE_GetObj<ObjCamMngr>(sceneID, "ObjCamMngr")))
    camMngr->SetCamCenter(pos.x + 8, pos.y + 16);
}

#define ACC ( \
  ((int)(ESGE_deltaTm * ESGE_deltaTm)) * 0x0020 / 256 \
)
#define VEL (((int)ESGE_deltaTm) * 0x0140 / 16)
#define GRA ( \
  ((int)(ESGE_deltaTm * ESGE_deltaTm)) * 0x0010 / 256 \
)
#define JMP (((int)ESGE_deltaTm) * 0x0280 / 16)
#define ROUND(T, S, N) ( \
  ((N + (1<<(S-1)) + (N>>(sizeof(T)*8-1))) & (~((1<<S)-1))) >> S \
)

void
ObjPlayer::OnUpdate(void)
{
  const Uint8 *keys;

  if (fVel.y + fAcc.y >= JMP)
  {
    fAcc.y = 0;
    fVel.y = JMP;
  }
  else fAcc.y = GRA;

  keys = SDL_GetKeyboardState(NULL);
    
  if (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT])
  {
    if (fVel.x - ACC < -VEL)
    {
      fAcc.x = 0;
      fVel.x = -VEL;
    }
    else fAcc.x = -ACC;

    facingR = false;
  }
  else if (!keys[SDL_SCANCODE_LEFT] && keys[SDL_SCANCODE_RIGHT])
  {
    if (fVel.x + ACC > VEL)
    {
      fAcc.x = 0;
      fVel.x = VEL;
    }
    else fAcc.x = ACC;

    facingR = true;
  }
  else
  {
    if (fVel.x > 0)
    {
      if (fVel.x - ACC <= 0)
      {
        fAcc.x = 0;
        fVel.x = 0;
      }
      else fAcc.x = -ACC;
    }
    else if ((fVel.x < 0))
    {
      if (fVel.x + ACC >= 0)
      {
        fAcc.x = 0;
        fVel.x = 0;
      }
      else fAcc.x = ACC;
    }
    else
    {
      fAcc.x = 0;
      fVel.x = 0;
    }
  }

  if (keys[SDL_SCANCODE_SPACE])
  {
    if (fVel.y >> POS_SCALE == 0)
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
        fVel.y = -JMP;
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

  pos.x = ROUND(int, POS_SCALE, fPos.x);
  pos.y = ROUND(int, POS_SCALE, fPos.y);

  if (facingR)
  {
    if (fVel.x == 0)
    {
      if (animPlayer.anim != anims + STAND_R)
        animPlayer.Start(anims + STAND_R);
    }
    else
    {
      if (animPlayer.anim != anims + RUN_R)
        animPlayer.Start(anims + RUN_R);
    }
  }
  else
  {
    if (fVel.x == 0)
    {
      if (animPlayer.anim != anims + STAND_L)
        animPlayer.Start(anims + STAND_L);
    }
    else
    {
      if (animPlayer.anim != anims + RUN_L)
        animPlayer.Start(anims + RUN_L);
    }
  }

  animPlayer.Update(ESGE_deltaTm);
  animPlayer.GetSprite(&sprite);
}

void
ObjPlayer::OnPhysic(void)
{
  ObjRoomMngr *roomMngr;
  ObjCamMngr *camMngr;

  ESGE_ObjDynamic::OnPhysic();

  if ((roomMngr = ESGE_GetObj<ObjRoomMngr>(sceneID, "ObjRoomMngr")))
    roomMngr->SetFocusCenter(pos.x + 8, pos.y + 16);

  if ((camMngr = ESGE_GetObj<ObjCamMngr>(sceneID, "ObjCamMngr")))
    camMngr->SetCamCenter(pos.x + 8, pos.y + 16);
}

void
ObjPlayer::OnCollide(ESGE_ObjCollider *other)
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
    fPos.x = pos.x << POS_SCALE;
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
    fPos.y = pos.y << POS_SCALE;
    fVel.y = 0;
  }
}

void
ObjPlayer::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();
  EnableUpdate();
  EnableDynamic();
  EnableDraw();

  ESGE_ShareObj<ObjPlayer>(this);
}
void
ObjPlayer::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();
  DisableUpdate();
  DisableDynamic();
  DisableDraw();

  ESGE_UnshareObj<ObjPlayer>(this);
}

#ifdef ESGE_EDITOR
void
ObjPlayer::OnEditorInit(void)
{
  EnableDraw();
}
void
ObjPlayer::OnEditorQuit(void)
{
  DisableDraw();
}
#endif

ESGE_TYPE_FIELDS(
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
      ESGE_END_FIELD
    )
  )
)
