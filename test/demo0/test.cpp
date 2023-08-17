#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_file.h"
#include "ESGE_spritesheet.h"
#include "ESGE_anim.h"
#include "ESGE_time.h"

#include "ESGE_objEvent.h"
#include "ESGE_objUpdate.h"
#include "ESGE_objStatic.h"
#include "ESGE_objDynamic.h"
#include "ESGE_objDrawSprite.h"

#include "ESGE_objStatic.h"





#define TILE_SS "tiles.sprite.bin"

class ObjTile:
  public ESGE_ObjScene,
  public ESGE_ObjDrawSprite,
  public ESGE_ObjStatic
{
public:
  ESGE_Spritesheet *spritesheet;
  int spriteCol = 0, spriteRow = 0;


  static int GetCol(void *obj)
  {
    return ((ObjTile*)obj)->ESGE_ObjStatic::GetCol();
  }
  static void SetCol(void *obj, int value)
  {
    ((ObjTile*)obj)->ESGE_ObjStatic::SetCol(value);
  }

  static int GetRow(void *obj)
  {
    return ((ObjTile*)obj)->ESGE_ObjStatic::GetRow();
  }
  static void SetRow(void *obj, int value)
  {
    ((ObjTile*)obj)->ESGE_ObjStatic::SetRow(value);
  }

  static int GetSpriteCol(void *obj)
  {
    return ((ObjTile*)obj)->spriteCol;
  }
  static void SetSpriteCol(void *obj, int value)
  {
    ((ObjTile*)obj)->spriteCol = value;

    ((ObjTile*)obj)->spritesheet->GetSprite(
      &((ObjTile*)obj)->sprite,
      value,
      ((ObjTile*)obj)->spriteRow
    );
  }

  static int GetSpriteRow(void *obj)
  {
    return ((ObjTile*)obj)->spriteRow;
  }
  static void SetSpriteRow(void *obj, int value)
  {
    ((ObjTile*)obj)->spriteRow = value;

    ((ObjTile*)obj)->spritesheet->GetSprite(
      &((ObjTile*)obj)->sprite,
      ((ObjTile*)obj)->spriteCol,
      value
    );
  }


  ObjTile(void)
  {
    layer = 1;
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(TILE_SS);
    spritesheet->GetSprite(&sprite, spriteCol, spriteRow);

    offsetSize.w = 16;
    offsetSize.h = 16;
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
    col,
    ObjTile::GetCol,
    ObjTile::SetCol,
    ESGE_FIELD(
      I,
      row,
      ObjTile::GetRow,
      ObjTile::SetRow,
      ESGE_FIELD(
        I,
        spriteCol,
        ObjTile::GetSpriteCol,
        ObjTile::SetSpriteCol,
        ESGE_FIELD(
          I,
          spriteRow,
          ObjTile::GetSpriteRow,
          ObjTile::SetSpriteRow,
          ESGE_NO_FIELD
        )
      )
    )
  )
)
