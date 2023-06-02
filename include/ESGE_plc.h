#ifndef ESGE_PLC_H_
# define ESGE_PLC_H_

# define ESGE_MAX_PRI 0u
# define ESGE_MIN_PRI (~0u)

# include <SDL2/SDL.h>


typedef void (*ESGE_PLC)(void*);

# ifdef __cplusplus
extern "C" {
# endif

SDL_bool ESGE_AnyPLC(void);
int ESGE_AddPLC(unsigned pri, ESGE_PLC f, void *d);
void ESGE_DelPLC(unsigned pri, ESGE_PLC f, void *d);
void ESGE_UpdatePLC(void);

# ifdef __cplusplus
}
# endif

#endif
