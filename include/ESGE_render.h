#ifndef ESGE_RENDER_H_
# define ESGE_RENDER_H_

# include <SDL2/SDL.h>
# include "ESGE_plc.h"

# define ESGE_DRAW_LAYERS_PRI ESGE_MIN_PRI

typedef void (*ESGE_DrawCallback)(void *userdata, SDL_Renderer *rend);

# ifdef __cplusplus
extern "C" {
# endif

int ESGE_AddDrawCallback(
  unsigned layer,
  SDL_Renderer *rend,
  ESGE_DrawCallback draw,
  void *userdata
);

void ESGE_DelDrawCallback(
  unsigned layer,
  SDL_Renderer *rend,
  ESGE_DrawCallback draw,
  void *userdata
);

# ifdef __cplusplus
}
# endif

#endif