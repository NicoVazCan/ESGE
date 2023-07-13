#ifndef ESGE_HASH_H_
# define ESGE_HASH_H_

# include <SDL2/SDL.h>

# ifdef __cplusplus
extern "C" {
# endif

SDL_FORCE_INLINE Uint64
ESGE_Hash(const char *str)
{
  Uint64 hash = 5381;
  int c;

  while ((c = *str++)) hash = ((hash << 5) + hash) + c;

  return hash;
}

# ifdef __cplusplus
}
# endif

#endif