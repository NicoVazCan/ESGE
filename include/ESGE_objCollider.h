/**
 * \file ESGE_objCollider.h
 * \brief Contains the definition of the ESGE_ObjCollider class which represents a game object with a box which can collide physically with others.
 */
#ifndef ESGE_OBJ_COLLIDER_H_
# define ESGE_OBJ_COLLIDER_H_

# include <SDL2/SDL.h>
# include "ESGE_objBox.h"


/**
 * \class ESGE_ObjCollider
 * \brief Represents a game object with a box which can collide physically with others.
 * Inherits from ESGE_ObjBox in a non-virtual way, since the same game object may have
 * several boxes of different sizes that fulfill different functions.
 */
class ESGE_ObjCollider: public ESGE_ObjBox
{
public:
  /**
   * \brief Default constructor for ESGE_ObjCollider.
   */
  ESGE_ObjCollider(void);
  /**
   * \brief Virtual destructor for ESGE_ObjCollider.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjCollider(void) = 0;

  /**
   * \brief Callback function when collision happens from the top side.
   * This is called from the other collider.
   * 
   * \param other The other ESGE_ObjCollider object that collides with this one.
   */
  virtual void OnCollideUp(ESGE_ObjCollider *other);
  /**
   * \brief Callback function when collision happens from the bottom side.
   * This is called from the other collider.
   * 
   * \param other The other ESGE_ObjCollider object that collides with this one.
   */
  virtual void OnCollideDown(ESGE_ObjCollider *other);
  /**
   * \brief Callback function when collision happens from the left side.
   * This is called from the other collider.
   * 
   * \param other The other ESGE_ObjCollider object that collides with this one.
   */
  virtual void OnCollideLeft(ESGE_ObjCollider *other);
  /**
   * \brief Callback function when collision happens from the right side.
   * This is called from the other collider.
   * 
   * \param other The other ESGE_ObjCollider object that collides with this one.
   */
  virtual void OnCollideRight(ESGE_ObjCollider *other);
};

#endif
