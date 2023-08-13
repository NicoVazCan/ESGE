#include "ESGE_objDrawSprite.h"


ESGE_ObjDrawSprite::ESGE_ObjDrawSprite(void)
{}

ESGE_ObjDrawSprite::~ESGE_ObjDrawSprite(void)
{}


void
ESGE_ObjDrawSprite::OnDraw(void)
{
  ESGE_Display::WorldDrawSprite(&sprite, pos);
}