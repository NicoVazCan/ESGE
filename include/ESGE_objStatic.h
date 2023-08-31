/**
 * \file ESGE_objStatic.h
 * \brief Contains the definition of the ESGE_ObjStatic class which represents static game objects with cell-based positioning and collision behavior.
 */
#ifndef ESGE_OBJ_STATIC_H_
# define ESGE_OBJ_STATIC_H_

# include "ESGE_objCell.h"
# include "ESGE_objCollider.h"



/**
 * \struct ESGE_ListV
 * \brief Represents a vertical list of static game objects.
 */
struct ESGE_ListV
{
  unsigned row; /**< The row index. */
  class ESGE_ObjStatic *obj; /**< Pointer to the static game object at this cell. */
  ESGE_ListV *next; /**< Pointer to the next vertical list item. */
};

/**
 * \struct ESGE_ListH
 * \brief Represents a horizontal list of static game objects.
 */
struct ESGE_ListH
{
  unsigned col; /**< The column index. */
  ESGE_ListV *listV; /**< Pointer to the vertical list of static game objects. */
  ESGE_ListH *next; /**< Pointer to the next horizontal list item. */
};


/**
 * \class ESGE_ObjStatic
 * \brief Represents static objects with cell-based positioning and collision behavior.
 * Inherits from ESGE_ObjCell for scene cell positioning and ESGE_ObjCollider for collision behavior.
 */
class ESGE_ObjStatic: public ESGE_ObjCell, public ESGE_ObjCollider
{
  static ESGE_ListH *listH; /**< Pointer to the first horizontal list item. */

  bool enabledStatic = false; /**< Flag indicating if the object is enabled as a static game object. */

public:
  /**
   * \brief Static method to get the static game object at a given position.
   * \param pos The position to check.
   * \return Pointer to the ESGE_ObjStatic game object at the given position, or NULL if none found.
   */
  static ESGE_ObjStatic *GetObjAt(SDL_Point pos);
  /**
   * \brief Static method to get the static object above the given box.
   * \param box The box to check.
   * \return Pointer to the ESGE_ObjStatic game object above the given box, or NULL if none found.
   */
  static ESGE_ObjStatic *GetObjAtUp(SDL_Rect box);
  /**
   * \brief Static method to get the static game object below the given box.
   * \param box The box to check.
   * \return Pointer to the ESGE_ObjStatic game object below the given box, or NULL if none found.
   */
  static ESGE_ObjStatic *GetObjAtDown(SDL_Rect box);
  /**
   * \brief Static method to get the static game object to the left of the given box.
   * \param box The box to check.
   * \return Pointer to the ESGE_ObjStatic game object to the left of the given box, or NULL if none found.
   */
  static ESGE_ObjStatic *GetObjAtLeft(SDL_Rect box);
  /**
   * \brief Static method to get the static game object to the right of the given box.
   * \param box The box to check.
   * \return Pointer to the ESGE_ObjStatic game object to the right of the given box, or NULL if none found.
   */
  static ESGE_ObjStatic *GetObjAtRight(SDL_Rect box);

  /**
   * \brief Default constructor for ESGE_ObjStatic.
   */
  ESGE_ObjStatic(void);
  /**
   * \brief Virtual destructor for ESGE_ObjStatic.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjStatic(void) = 0;

  /**
   * \brief Get the column number with of the static game object.
   * \return The column number width.
   */
  int GetColW(void);
  /**
   * \brief Set the column number width of the static game object.
   * \param colW The new colum number width to set.
   */
  void SetColW(int colW);

  /**
   * \brief Get the row number height of the static game object.
   * \return The row number height.
   */
  int GetRowH(void);
  /**
   * \brief Set the row number height of the static game object.
   * \param rowH The new row number height to set.
   */
  void SetRowH(int rowH);

  /**
   * \brief Enable the object as a static object.
   * Adds the game object to the lookup table.
   * Must be not already in the table and the cell must also not be
   * occupied with other game object, in this case the program will abort.
   */
  void EnableStatic(void);
  /**
   * \brief Disable the game object as a static object.
   * Deletes the game object from the lookup table.
   * Must be enabled first.
   */
  void DisableStatic(void);
  /**
   * \brief Check if the game object is enabled as a static game object.
   * \return True if the game object is enabled as a static game object, false otherwise.
   */
  bool IsEnabledStatic(void);

  /**
   * \brief Override of the OnCollideUp function to handle collision behavior from above.
   * \param other The other collider game object that collides with this one.
   */
  virtual void OnCollideUp(ESGE_ObjCollider *other) override;
  /**
   * \brief Override of the OnCollideDown function to handle collision behavior from below.
   * \param other The other collider game object that collides with this one.
   */
  virtual void OnCollideDown(ESGE_ObjCollider *other) override;
  /**
   * \brief Override of the OnCollideLeft function to handle collision behavior from the left side.
   * \param other The other collider game object that collides with this one.
   */
  virtual void OnCollideLeft(ESGE_ObjCollider *other) override;
  /**
   * \brief Override of the OnCollideLeft function to handle collision behavior from the right side.
   * \param other The other collider game object that collides with this one.
   */
  virtual void OnCollideRight(ESGE_ObjCollider *other) override;
};

#endif