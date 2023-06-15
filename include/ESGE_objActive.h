#ifndef ESGE_OBJ_ACTIVE_H_
# define ESGE_OBJ_ACTIVE_H_

# include <SDL2/SDL.h>

class ESGE_ObjActive
{
  bool enabled = false;
public:
  ESGE_ObjActive() = default;
  virtual ~ESGE_ObjActive() = 0;
  virtual void OnEnable(void);
  virtual void OnDisable(void);
  bool IsEnabled(void) const;
};

#endif