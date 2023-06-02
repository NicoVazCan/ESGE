#ifndef ESGE_EVENT_H_
# define ESGE_EVENT_H_

# define ESGE_EVENT_LOOP_PRI ESGE_MAX_PRI

# include <SDL2/SDL.h>
#include "ESGE_plc.h"

typedef void(*ESGE_EventWatch)(void *userdata, SDL_Event *event);

# ifdef __cplusplus
extern "C" {
# endif

int ESGE_AddEventWatch(
  Uint32 type,
  ESGE_EventWatch watch,
  void *userdata
);

void ESGE_DelEventWatch(
  Uint32 type,
  ESGE_EventWatch watch,
  void *userdata
);

# ifdef __cplusplus
}
# endif

#endif
