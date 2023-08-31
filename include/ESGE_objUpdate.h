/**
 * \file ESGE_objUpdate.h
 * \brief Contains the definition of the ESGE_ObjUpdate class which represents objects with update behavior.
 */
#ifndef ESGE_OBJ_UPDATE_H_
# define ESGE_OBJ_UPDATE_H_

/**
 * \class ESGE_ObjUpdate
 * \brief Represents objects with update behavior.
 */
class ESGE_ObjUpdate
{
  static ESGE_ObjUpdate *list; /**< Pointer to the first game object with update behavior in the list. */

  ESGE_ObjUpdate *next; /**< Pointer to the next game object with update behavior in the list. */
  bool enabledUpdate = false; /**< Flag indicating if update is enabled for this game object. */

public:
  /**
   * \brief Static method to update all enabled objects in the list.
   * Internal use only.
   * Called in the main loop just after event loop.
   */
  static void Update(void);

  /**
   * \brief Default constructor for ESGE_ObjUpdate.
   */
  ESGE_ObjUpdate(void);
  /**
   * \brief Virtual destructor for ESGE_ObjUpdate.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjUpdate(void) = 0;

  /**
   * \brief Enable update for the game object.
   */
  void EnableUpdate(void);
  /**
   * \brief Disable update for the game object.
   */
  void DisableUpdate(void);
  /**
   * \brief Check if update is enabled for the game object.
   * \return True if update is enabled, false otherwise.
   */
  bool IsEnabledUpdate(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when the game object needs to be updated
   * just after the event loop.
   */
  virtual void OnUpdate(void);
};

#endif