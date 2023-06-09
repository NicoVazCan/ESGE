#ifndef ESGE_OBJ_ACTIVE_H_
# define ESGE_OBJ_ACTIVE_H_

# include <SDL2/SDL.h>

class ESGE_ObjActive
{
  SDL_bool enabled;
public:
  ESGE_ObjActive() = default;
  virtual ~ESGE_ObjActive() = 0;
  virtual void OnEnable(void);
  virtual void OnDisable(void);
  SDL_bool IsEnabled(void) const;
};

#endif