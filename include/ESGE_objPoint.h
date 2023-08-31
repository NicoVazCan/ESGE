/**
 * \file ESGE_objPoint.h
 * \brief Contains the definition of the ESGE_ObjPoint class.
 */
#ifndef ESGE_OBJ_POINT_H_
# define ESGE_OBJ_POINT_H_

# include <SDL2/SDL.h>

/**
 * \class ESGE_ObjPoint
 * \brief Abstract base class to represent a game object with
 * position in the scene.
 */
class ESGE_ObjPoint
{
public:
  /**
   * The position of the point in the scene.
   */
  SDL_Point pos = {0, 0}; 

  /**
   * \brief Default constructor.
   *
   * Initializes the point's position to (0, 0).
   */
  ESGE_ObjPoint(void);
  /**
   * \brief Pure virtual destructor.
   *
   * Defines a pure virtual destructor to make the class an abstract
   * base class.
   */
  virtual ~ESGE_ObjPoint(void) = 0;
};

#endif