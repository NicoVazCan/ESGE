/**
 * \file ESGE_objPhysic.h
 * \brief Contains the definition of the ESGE_ObjPhysic class which represents game objects with physics behavior.
 */
#ifndef ESGE_OBJ_PHYSIC_H_
# define ESGE_OBJ_PHYSIC_H_


/**
 * \class ESGE_ObjPhysic
 * \brief Represents game objects with physics behavior.
 */
class ESGE_ObjPhysic
{
  static ESGE_ObjPhysic *list; /**< Pointer to the first game object with physics behavior in the list. */

  ESGE_ObjPhysic *next; /**< Pointer to the next game object with physics behavior in the list. */
  bool enabledPhysic = false; /**< Flag indicating if physics simulation is enabled for this game object. */

public:
  /**
   * \brief Static method to perform physics simulation on all enabled game objects in the list.
   * Internal use only.
   * Called in the main loop just after update pipeline.
   */
  static void Physic(void);

  /**
   * \brief Default constructor for ESGE_ObjPhysic.
   */
  ESGE_ObjPhysic(void);
  /**
   * \brief Virtual destructor for ESGE_ObjPhysic.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjPhysic(void) = 0;

  /**
   * \brief Enable physics simulation for the game object.
   */
  void EnablePhysic(void);
  /**
   * \brief Disable physics simulation for the game object.
   */
  void DisablePhysic(void);
  /**
   * \brief Check if physics simulation is enabled for the object.
   * \return True if physics simulation is enabled, false otherwise.
   */
  bool IsEnabledPhysic(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when the physics simulation needs to be
   * applied to the game object just after updating.
   */
  virtual void OnPhysic(void);
};

#endif