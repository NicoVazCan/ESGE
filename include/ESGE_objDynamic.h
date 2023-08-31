/**
 * \file ESGE_objDynamic.h
 * \brief Contains the definition of the ESGE_ObjDynamic class which represents dynamic game objects with physics and collision behavior.
 */
#ifndef ESGE_OBJ_DYNAMIC_H_
# define ESGE_OBJ_DYNAMIC_H_

# include <SDL2/SDL.h>
# include "ESGE_objPhysic.h"
# include "ESGE_objCollider.h"

/**
 * \class ESGE_ObjDynamic
 * \brief Represents dynamic game objects with physics and collision behavior.
 * Inherits from ESGE_ObjPhysic for physics behavior and ESGE_ObjCollider for collision behavior.
 */
class ESGE_ObjDynamic: public ESGE_ObjPhysic, public ESGE_ObjCollider
{
public:
  /**
   * The previous position of the dynamic game object.
   * Used to calculate its relative speed, needed to collision detection.
   * At the game object start, mus be equal to ESGE_ObjPoint::pos.
   */
  SDL_Point prevPos = {0, 0};
  
  /**
   * \brief Default constructor for ESGE_ObjDynamic.
   */
  ESGE_ObjDynamic(void);
  /**
   * \brief Virtual destructor for ESGE_ObjDynamic.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjDynamic(void) = 0;

  /**
   * \brief Override of the OnPhysic function to handle physics behavior.
   * The dynamic game object will only collide with static game objects for simplicity.
   */
  virtual void OnPhysic(void) override;
};

#endif