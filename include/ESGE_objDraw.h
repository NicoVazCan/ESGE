#ifndef ESGE_OBJ_DRAW_H_
# define ESGE_OBJ_DRAW_H_


class ESGE_ObjDraw
{
  static ESGE_ObjDraw *list;

  ESGE_ObjDraw *next;
  bool enabledDraw = false;

public:
  int layer;

  static void Draw(void);

  ESGE_ObjDraw(void);
  virtual ~ESGE_ObjDraw(void) = 0;

  void EnableDraw(void);
  void DisableDraw(void);
  bool IsEnabledDraw(void);

  virtual void OnDraw(void);
};

#endif