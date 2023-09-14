/**
 * \file ESGE_time.h
 * \brief Declaration of time-related variables.
 */
#ifndef ESGE_TIME_H_
# define ESGE_TIME_H_

/**
 * \brief Fixed time elapsed since the last frame (in micro senconds).
 * \details This variable stores the fixed time elapsed since the last frame update. Used for time based actions.
 */
extern Uint32 ESGE_deltaTm;
/**
 * \brief Potential time elapsed since the last frame (in CPU counter).
 * \details This variable stores the actual time elapsed since the last frame update before sleep. Used for profiling.
 * 
 * \sa SDL_GetPerformanceFrequency()
 */
extern Uint64 ESGE_deltaCnt;

#endif