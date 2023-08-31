/**
 * \file ESGE_error.h
 * \brief Contains exit and error handling macros.
 */

#ifndef ESGE_ERROR_H_
# define ESGE_ERROR_H_

# include <SDL2/SDL.h>

/**
 * \def ESGE_Exit
 * \brief Macro to handle program exit based on platform.
 *
 * Provides platform-specific exit mechanism based on the current platform.
 * Do not call this macro, internal use only.
 *
 * \param exitcode Exit code for the program.
 */
# if defined(__WIN32__) || defined(__GDK__)
#   define ESGE_Exit(exitcode) { \
  TerminateProcess(GetCurrentProcess(), exitcode); \
  ExitProcess(exitcode); \
}
# elif defined(__EMSCRIPTEN__)
#   define ESGE_Exit(exitcode) { \
  emscripten_cancel_main_loop(); \
  emscripten_force_exit(exitcode); \
  exit(exitcode); \
}
# elif defined(__HAIKU__)
#   define ESGE_Exit(exitcode) _exit(exitcode)
# elif defined(HAVE__EXIT)
#   define ESGE_Exit(exitcode) _Exit(exitcode)
# else
#   define ESGE_Exit(exitcode) _exit(exitcode)
# endif

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
  SDL_Quit(); \
  ESGE_Exit(42); \
}

#endif