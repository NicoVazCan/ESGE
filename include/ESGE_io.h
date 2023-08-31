/**
 * \file ESGE_io.h
 * \brief Contains input and output functions for SDL_RWops (stream).
 */
#ifndef ESGE_IO_H_
# define ESGE_IO_H_

#include <SDL2/SDL.h>

# ifdef __cplusplus
extern "C" {
# endif

/**
 * \brief Read a signed 8-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Sint8  ESGE_ReadS8(SDL_RWops *io);
/**
 * \brief Read an unsigned 8-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Uint8  ESGE_ReadU8(SDL_RWops *io);
/**
 * \brief Read a signed 16-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Sint16 ESGE_ReadS16(SDL_RWops *io);
/**
 * \brief Read an unsigned 16-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Uint16 ESGE_ReadU16(SDL_RWops *io);
/**
 * \brief Read a signed 32-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Sint32 ESGE_ReadS32(SDL_RWops *io);
/**
 * \brief Read an unsigned 32-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Uint32 ESGE_ReadU32(SDL_RWops *io);
/**
 * \brief Read a signed 64-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Sint64 ESGE_ReadS64(SDL_RWops *io);
/**
 * \brief Read an unsigned 64-bit integer in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
Uint64 ESGE_ReadU64(SDL_RWops *io);
/**
 * \brief Read a float value in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
float  ESGE_ReadFloat(SDL_RWops *io);
/**
 * \brief Read a string in binary from an SDL_RWops stream.
 * If a failure occurs during reading, the program execution will be aborted.
 * The operation is endian-independent.
 */
char*  ESGE_ReadStr(SDL_RWops *io, char *str, size_t n);

/**
 * \brief Write in binary a signed 8-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteS8(SDL_RWops *io, Sint8  value);
/**
 * \brief Write in binary an unsigned 8-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteU8(SDL_RWops *io, Uint8  value);
/**
 * \brief Write in binary a signed 16-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteS16(SDL_RWops *io, Sint16 value);
/**
 * \brief Write in binary an unsigned 16-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteU16(SDL_RWops *io, Uint16 value);
/**
 * \brief Write in binary a signed 32-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteS32(SDL_RWops *io, Sint32 value);
/**
 * \brief Write in binary an unsigned 32-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteU32(SDL_RWops *io, Uint32 value);
/**
 * \brief Write in binary a signed 64-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteS64(SDL_RWops *io, Sint64 value);
/**
 * \brief Write in binary an unsigned 64-bit integer to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteU64(SDL_RWops *io, Uint64 value);
/**
 * \brief Write in binary a float value to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteFloat(SDL_RWops *io, float  value);
/**
 * \brief Write in binary in binary a string to an SDL_RWops stream.
 * If a failure occurs during writing, the program execution will be aborted.
 * The operation is endian-independent.
 */
void ESGE_WriteStr(SDL_RWops *io, const char *str);

# ifdef __cplusplus
}
# endif

#endif