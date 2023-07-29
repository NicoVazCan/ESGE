#ifndef ESGE_OBJ_DRAW_H_
# define ESGE_OBJ_DRAW_H_

# include <SDL2/SDL.h>

class ESGE_ObjDraw
{
  static ESGE_ObjDraw *list;

  ESGE_ObjDraw *next = NULL;

public:
  unsigned layer;

  static void Draw(void);

  ESGE_ObjDraw(void);
  virtual ~ESGE_ObjDraw(void) = 0;

  void EnableDraw(void);
  void DisableDraw(void);
  bool IsEnabledDraw(void);

  virtual void OnDraw(void);
};

#endif