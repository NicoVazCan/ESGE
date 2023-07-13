#ifndef ESGE_ERROR_H_
# define ESGE_ERROR_H_

# include <SDL2/SDL.h>

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
#   define ESGE_Exit(exitcode) _Exit(exitcode);
# else
#   define ESGE_Exit(exitcode) _exit(exitcode);
# endif

# define ESGE_Error(fmt, ...) { \
  SDL_LogError(0, "%s:%s:%d:", SDL_FILE, SDL_FUNCTION, SDL_LINE); \
  SDL_LogError(0, (fmt), ##__VA_ARGS__); \
  ESGE_Exit(42); \
}

#endif