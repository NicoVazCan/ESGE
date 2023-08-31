/**
 * \file ESGE_objBox.h
 * \brief Contains the definition of the ESGE_ObjBox class which represents a game object with a box around to check overlap with others.
 */
#ifndef ESGE_OBJ_BOX_H_
# define ESGE_OBJ_BOX_H_

# include <SDL2/SDL.h>
# include "ESGE_objPoint.h"

/**
 * \class ESGE_ObjBox
 * \brief Represents game object with a box around to check overlap with others.
 * Inherits from ESGE_ObjPoint for scene position functionality.
 */
class ESGE_ObjBox: public virtual ESGE_ObjPoint
{
public:
  SDL_Rect offsetSize = {0, 0, 0, 0}; /**< The offset and size of the box. */
  
  /**
   * \brief Default constructor for ESGE_ObjBox.
   */
  ESGE_ObjBox(void);
  /**
   * \brief Virtual destructor for ESGE_ObjBox.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjBox(void) = 0;

  /**
   * \brief Get the game object box based in its position.
   * \return The game object box based in its position.
   */
  inline SDL_Rect GetBox(void)
  {
    SDL_Rect box = offsetSize;

    box.x += pos.x;
    box.y += pos.y;

    return box;
  }
  /**
   * \brief Set the position of the game object based in the new box.
   * \param box The new game object box.
   */
  inline void SetBox(SDL_Rect box)
  {
    pos.x = box.x - offsetSize.x;
    pos.y = box.y - offsetSize.y;
  }
};

#endif