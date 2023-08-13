#ifndef ESGE_HASH_H_
# define ESGE_HASH_H_

# include <SDL2/SDL.h>

#define ESGE_Hash(STR) ESGE_RecHash(STR, 5381)

static constexpr Uint64
ESGE_RecHash(const char *str, Uint64 hash)
{
  return (
    *str ? ESGE_RecHash(str+1, ((hash << 5) + hash) + *str) : hash
  );
}

#endif