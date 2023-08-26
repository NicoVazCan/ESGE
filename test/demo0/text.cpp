#include "text.h"
#include "player.h"
#include <SDL2/SDL.h>
#include "ESGE_spritesheet.h"
#include "ESGE_file.h"
#include "ESGE_display.h"

ESGE_TYPE_FIELDS(
  ObjText,
  ESGE_FIELD(
    I,
    pos.x,
    ObjText::GetPosX,
    ObjText::SetPosX,
    ESGE_FIELD(
      I,
      pos.y,
      ObjText::GetPosY,
      ObjText::SetPosY,
      ESGE_FIELD(
        S,
        str,
        ObjText::GetStr,
        ObjText::SetStr,
        ESGE_END_FIELD
      )
    )
  )
)

int
ObjText::GetPosX(void *obj)
{
  return ((ObjText*)obj)->pos.x;
}
void
ObjText::SetPosX(void *obj, int value)
{
  ((ObjText*)obj)->pos.x = value;
}

int
ObjText::GetPosY(void *obj)
{
  return ((ObjText*)obj)->pos.y;
}
void
ObjText::SetPosY(void *obj, int value)
{
  ((ObjText*)obj)->pos.y = value;
}

char*
ObjText::GetStr(void *obj, char *str, size_t n)
{
  SDL_strlcpy(str, ((ObjText*)obj)->str, n);
  return str;
}
void
ObjText::SetStr(void *obj, const char *str)
{
  SDL_strlcpy(((ObjText*)obj)->str, str, TEXT_LEN);
}

#define TEXT_SS "sprites/font.sprite.bin"

ObjText::ObjText(void)
{
  layer = PLAYER_LAYER+20;
  spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(TEXT_SS);
}
ObjText::~ObjText(void)
{
  ESGE_FileMngr<ESGE_Spritesheet>::Leave(spritesheet);
}

#define CHAR_W 8
#define CHAR_H 8

void
ObjText::OnDraw(void)
{
  ESGE_Sprite sprite;
  SDL_Point offset = {0, 0};

  for (char *c = str; *c != '\0'; ++c)
  {
    switch (*c)
    {
      SDL_Point charPos;

    case '\n':
      offset.x = 0;
      offset.y += CHAR_H;
      break;
    default:
      spritesheet->GetSprite(&sprite, *c, 0);

      charPos.x = pos.x + offset.x;
      charPos.y = pos.y + offset.y;

      ESGE_Display::DisplayDrawSprite(&sprite, charPos);

      offset.x += CHAR_W;
    }
  }
}

void
ObjText::OnEnable(void)
{
  ESGE_ObjScene::OnEnable();

  EnableDraw();

  ESGE_ShareObj<ObjText>(this);
}
void
ObjText::OnDisable(void)
{
  ESGE_ObjScene::OnDisable();

  DisableDraw();

  ESGE_UnshareObj<ObjText>(this);
}

#ifdef ESGE_EDITOR
void
ObjText::OnEditorEnable(void)
{
  EnableDraw();
}
void
ObjText::OnEditorDisable(void)
{
  DisableDraw();
}
#endif