#include <SDL2/SDL.h>
#include "ESGE_scene.h"
#include "ESGE_display.h"
#include "ESGE_hash.h"

#include "ESGE_objStatic.h"
#include "ESGE_objDraw.h"


class ObjTileCol:
  public ESGE_ObjScene,
  public ESGE_ObjDraw,
  public ESGE_ObjStatic
{
public:
  static int GetCol(void *obj)
  {
    return ((ObjTileCol*)obj)->ESGE_ObjStatic::GetCol();
  }
  static void SetCol(void *obj, int value)
  {
    ((ObjTileCol*)obj)->ESGE_ObjStatic::SetCol(value);
  }

  static int GetRow(void *obj)
  {
    return ((ObjTileCol*)obj)->ESGE_ObjStatic::GetRow();
  }
  static void SetRow(void *obj, int value)
  {
    ((ObjTileCol*)obj)->ESGE_ObjStatic::SetRow(value);
  }


  static int GetColW(void *obj)
  {
    return ((ObjTileCol*)obj)->ESGE_ObjStatic::GetColW();
  }
  static void SetColW(void *obj, int value)
  {
    ((ObjTileCol*)obj)->ESGE_ObjStatic::SetColW(value);
  }

  static int GetRowH(void *obj)
  {
    return ((ObjTileCol*)obj)->ESGE_ObjStatic::GetRowH();
  }
  static void SetRowH(void *obj, int value)
  {
    ((ObjTileCol*)obj)->ESGE_ObjStatic::SetRowH(value);
  }


  ObjTileCol(void)
  {
    layer = 2;
    ESGE_ObjStatic::SetColW(1);
    ESGE_ObjStatic::SetRowH(1);
  }
  virtual ~ObjTileCol(void)
  {}

  virtual void OnDraw(void)
  {
    ESGE_Display::WorldDrawRect(
      ESGE_ObjCollider::GetBox(),
      255,
      0,
      0,
      255
    );
  }


#ifdef ESGE_EDITOR
  virtual void OnEditorEnable(void)
  {
    ESGE_ObjScene::OnEnable();
    EnableDraw();
  }

  virtual void OnEditorDisable(void)
  {
    ESGE_ObjScene::OnDisable();
    DisableDraw();
  }
#endif

  virtual void OnEnable(void)
  {
    ESGE_ObjScene::OnEnable();
    //EnableDraw();
    EnableStatic();
  }

  virtual void OnDisable(void)
  {
    ESGE_ObjScene::OnDisable();
    //DisableDraw();
    DisableStatic();
  }
};

ESGE_TYPE_FIELDS(
  ObjTileCol,
  ESGE_FIELD(
    I,
    col,
    ObjTileCol::GetCol,
    ObjTileCol::SetCol,
    ESGE_FIELD(
      I,
      row,
      ObjTileCol::GetRow,
      ObjTileCol::SetRow,
      ESGE_FIELD(
        I,
        colW,
        ObjTileCol::GetColW,
        ObjTileCol::SetColW,
        ESGE_FIELD(
          I,
          rowH,
          ObjTileCol::GetRowH,
          ObjTileCol::SetRowH,
          ESGE_END_FIELD
        )
      )
    )
  )
)