#ifndef TEXT_H_
# define TEXT_H_

# include "ESGE_scene.h"
# include "ESGE_objPoint.h"
# include "ESGE_objDraw.h"

# define TEXT_LEN 128

class ESGE_Spritesheet;

class ObjText:
  public ESGE_ObjScene,
  public ESGE_ObjPoint,
  public ESGE_ObjDraw
{
  char str[TEXT_LEN] = "";
  ESGE_Spritesheet *spritesheet;

public:
  static int GetPosX(void *obj);
  static void SetPosX(void *obj, int value);

  static int GetPosY(void *obj);
  static void SetPosY(void *obj, int value);

  static char* GetStr(void *obj, char *str, size_t n);
  static void SetStr(void *obj, const char *str);

  ObjText(void);
  virtual ~ObjText(void) override;
  
  virtual void OnDraw(void) override;

  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;

#ifdef ESGE_EDITOR
  virtual void OnEditorEnable(void) override;
  virtual void OnEditorDisable(void) override;
#endif  
};

#endif