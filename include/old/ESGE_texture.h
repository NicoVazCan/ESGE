#ifndef ESGE_TEXTURE_H_
# define ESGE_TEXTURE_H_

# include <SDL2/SDL.h>

class ESGE_Texture
{
public:
  SDL_Texture *const texture;

  ESGE_Texture(const char *file);
  ~ESGE_Texture(void);
};

#endif