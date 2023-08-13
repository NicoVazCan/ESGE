#include "ESGE_io.h"

#include "ESGE_error.h"


Sint8
ESGE_ReadS8(SDL_RWops *io)
{
  Sint8 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return value;
}

Uint8
ESGE_ReadU8(SDL_RWops *io)
{
  Uint8 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return value;
}

Sint16
ESGE_ReadS16(SDL_RWops *io)
{
  Sint16 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE16(value);
}

Uint16
ESGE_ReadU16(SDL_RWops *io)
{
  Uint16 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE16(value);
}

Sint32
ESGE_ReadS32(SDL_RWops *io)
{
  Sint32 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE32(value);
}

Uint32
ESGE_ReadU32(SDL_RWops *io)
{
  Uint32 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE32(value);
}

Sint64
ESGE_ReadS64(SDL_RWops *io)
{
  Sint64 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE64(value);
}

Uint64
ESGE_ReadU64(SDL_RWops *io)
{
  Uint64 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE64(value);
}

float
ESGE_ReadFloat(SDL_RWops *io)
{
  float value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapFloatBE(value);
}

char*
ESGE_ReadStr(SDL_RWops *io, char *str, size_t n)
{
  char *c;

  SDL_assert(str);
  c = str;
  for (; n > 0 && (*c++ = ESGE_ReadU8(io)) != '\0'; --n);
  for (; n > 0; --n) *c++ = '\0';

  return str;
}


void
ESGE_WriteS8(SDL_RWops *io, Sint8 value)
{
  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU8(SDL_RWops *io, Uint8 value)
{
  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteS16(SDL_RWops *io, Sint16 value)
{
  value = SDL_SwapBE16(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU16(SDL_RWops *io, Uint16 value)
{
  value = SDL_SwapBE16(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteS32(SDL_RWops *io, Sint32 value)
{
  value = SDL_SwapBE32(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU32(SDL_RWops *io, Uint32 value)
{
  value = SDL_SwapBE32(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteS64(SDL_RWops *io, Sint64 value)
{
  value = SDL_SwapBE64(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU64(SDL_RWops *io, Uint64 value)
{
  value = SDL_SwapBE64(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteFloat(SDL_RWops *io, float value)
{
  value = SDL_SwapFloatBE(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteStr(SDL_RWops *io, const char *str)
{
  SDL_assert(str);

  while (*str != '\0') ESGE_WriteU8(io, *(Uint8*)str++);
}
