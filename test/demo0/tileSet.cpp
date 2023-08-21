#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_file.h"
#include "ESGE_spritesheet.h"

#include "ESGE_objCell.h"
#include "ESGE_objDrawSprite.h"


#define TILE_SET_SS "tiles.sprite.bin"


class ObjTileSet:
  public ESGE_ObjScene,
  public ESGE_ObjCell,
  public ESGE_ObjDrawSprite
{
  ESGE_Spritesheet *spritesheet;
  int spriteCol = 0, spriteRow = 0;

public:
  static int GetCol(void *obj)
  {
    return ((ObjTileSet*)obj)->ESGE_ObjCell::GetCol();
  }
  static void SetCol(void *obj, int value)
  {
    ((ObjTileSet*)obj)->ESGE_ObjCell::SetCol(value);
  }

  static int GetRow(void *obj)
  {
    return ((ObjTileSet*)obj)->ESGE_ObjCell::GetRow();
  }
  static void SetRow(void *obj, int value)
  {
    ((ObjTileSet*)obj)->ESGE_ObjCell::SetRow(value);
  }


  static int GetSpriteCol(void *obj)
  {
    return ((ObjTileSet*)obj)->spriteCol;
  }
  static void SetSpriteCol(void *obj, int value)
  {
    ((ObjTileSet*)obj)->spriteCol = value;

    ((ObjTileSet*)obj)->spritesheet->GetSprite(
      &((ObjTileSet*)obj)->sprite,
      value,
      ((ObjTileSet*)obj)->spriteRow
    );
  }

  static int GetSpriteRow(void *obj)
  {
    return ((ObjTileSet*)obj)->spriteRow;
  }
  static void SetSpriteRow(void *obj, int value)
  {
    ((ObjTileSet*)obj)->spriteRow = value;

    ((ObjTileSet*)obj)->spritesheet->GetSprite(
      &((ObjTileSet*)obj)->sprite,
      ((ObjTileSet*)obj)->spriteCol,
      value
    );
  }

    static int GetSpriteFlip(void *obj)
  {
    return ((ObjTileSet*)obj)->sprite.flip;
  }
  static void SetSpriteFlip(void *obj, int value)
  {
    ((ObjTileSet*)obj)->sprite.flip = (SDL_RendererFlip)value;
  }


  ObjTileSet(void)
  {
    layer = 1;
    spritesheet = ESGE_FileMngr<ESGE_Spritesheet>::Watch(TILE_SET_SS);
    spritesheet->GetSprite(&sprite, spriteCol, spriteRow);
  }
  virtual ~ObjTileSet(void) override
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
  }

  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableDraw();
  }
};


ESGE_TYPE_FIELDS(
  ObjTileSet,
  ESGE_FIELD(
    I,
    col,
    ObjTileSet::GetCol,
    ObjTileSet::SetCol,
    ESGE_FIELD(
      I,
      row,
      ObjTileSet::GetRow,
      ObjTileSet::SetRow,
      ESGE_FIELD(
        I,
        spriteCol,
        ObjTileSet::GetSpriteCol,
        ObjTileSet::SetSpriteCol,
        ESGE_FIELD(
          I,
          spriteRow,
          ObjTileSet::GetSpriteRow,
          ObjTileSet::SetSpriteRow,
          ESGE_FIELD(
            I,
            spriteFlip,
            ObjTileSet::GetSpriteFlip,
            ObjTileSet::SetSpriteFlip,
            ESGE_END_FIELD
          )
        )
      )
    )
  )
)
