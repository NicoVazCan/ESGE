#ifndef ESGE_IO_H_
# define ESGE_IO_H_

#include <SDL2/SDL.h>

# ifdef __cplusplus
extern "C" {
# endif

Sint8  ESGE_ReadS8(   SDL_RWops *io);
Uint8  ESGE_ReadU8(   SDL_RWops *io);
Sint16 ESGE_ReadS16(  SDL_RWops *io);
Uint16 ESGE_ReadU16(  SDL_RWops *io);
Sint32 ESGE_ReadS32(  SDL_RWops *io);
Uint32 ESGE_ReadU32(  SDL_RWops *io);
Sint64 ESGE_ReadS64(  SDL_RWops *io);
Uint64 ESGE_ReadU64(  SDL_RWops *io);
float  ESGE_ReadFloat(SDL_RWops *io);
char*  ESGE_ReadStr(  SDL_RWops *io, char *str, size_t n);

void ESGE_WriteS8(   SDL_RWops *io, Sint8  value);
void ESGE_WriteU8(   SDL_RWops *io, Uint8  value);
void ESGE_WriteS16(  SDL_RWops *io, Sint16 value);
void ESGE_WriteU16(  SDL_RWops *io, Uint16 value);
void ESGE_WriteS32(  SDL_RWops *io, Sint32 value);
void ESGE_WriteU32(  SDL_RWops *io, Uint32 value);
void ESGE_WriteS64(  SDL_RWops *io, Sint64 value);
void ESGE_WriteU64(  SDL_RWops *io, Uint64 value);
void ESGE_WriteFloat(SDL_RWops *io, float  value);
void ESGE_WriteStr(  SDL_RWops *io, const char *str);

# ifdef __cplusplus
}
# endif

#endif