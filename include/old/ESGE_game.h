#ifndef ESGE_GAME_H_
# define ESGE_GAME_H_

# include <SDL2/SDL.h>

# define ESGE_GAME_DEFAULT_DELTA 16

extern Uint32 ESGE_deltaTime;

# ifdef __cplusplus
extern "C" {
# endif

void ESGE_MainLoop(void);

# ifdef __cplusplus
}
# endif

#endif