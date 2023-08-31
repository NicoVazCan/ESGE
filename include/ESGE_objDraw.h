/**
 * \file ESGE_objDraw.h
 * \brief Contains the definition of the ESGE_ObjDraw class which
 * represents drawable game objects with layer-based drawing using Painter's algorithm.
 */
#ifndef ESGE_OBJ_DRAW_H_
# define ESGE_OBJ_DRAW_H_


/**
 * \class ESGE_ObjDraw
 * \brief Represents drawable game objects with layer-based drawing using Painter's algorithm.
 */
class ESGE_ObjDraw
{
  static ESGE_ObjDraw *list; /**< Pointer to the first drawable object in the sorted list. */

  ESGE_ObjDraw *next; /**< Pointer to the next drawable object in the sorted list. */
  bool enabledDraw = false; /**< Flag indicating if drawing is enabled for this game object. */

public:
  int layer = 0; /**< The drawing layer of the object. Objects with higher layer values are drawn on top. */

  /**
   * \brief Static method to draw all enabled drawable objects in the list.
   * Internal use only.
   * Called in the main loop just after physics pipeline.
   */
  static void Draw(void);

  /**
   * \brief Default constructor for ESGE_ObjDraw.
   */
  ESGE_ObjDraw(void);
  /**
   * \brief Virtual destructor for ESGE_ObjDraw.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjDraw(void) = 0;

  /**
   * \brief Enable drawing for the game object.
   */
  void EnableDraw(void);
  /**
   * \brief Disable drawing for the game object.
   */
  void DisableDraw(void);
  /**
   * \brief Check if drawing is enabled for the object.
   * \return True if drawing is enabled, false otherwise.
   */
  bool IsEnabledDraw(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when the game object needs to be drawn
   * just after physics.
   */
  virtual void OnDraw(void);
};

#endif