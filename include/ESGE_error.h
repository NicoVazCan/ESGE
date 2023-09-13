/**
 * \file ESGE_error.h
 * \brief Contains exit and error handling macros.
 */

#ifndef ESGE_ERROR_H_
# define ESGE_ERROR_H_

# include <SDL2/SDL.h>

/**
 * \def ESGE_Error
 * \brief Macro to handle and log an error.
 *
 * Logs an error message including file, function, and line information,
 * then quits the SDL subsystem and exits the program.
 * Call this macro when the failure is unrecoverable, similar to a
 * unchecked Java Exception.
 *
 * \param fmt Error message format string.
 * \param ... Additional arguments for the format string.
 */
# define ESGE_Error(fmt, ...) { \
  SDL_LogError(0, "%s:%s:%d:", SDL_FILE, SDL_FUNCTION, SDL_LINE); \
  SDL_LogError(0, (fmt), ##__VA_ARGS__); \
  SDL_TriggerBreakpoint(); \
}

#endif