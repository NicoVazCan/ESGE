/**
 * \file ESGE_objDrawSprite.h
 * \brief Contains the definition of the ESGE_ObjDrawSprite class which represents a drawable sprite game object with scene position and layer-based drawing using Painter's algorithm.
 */
#ifndef ESGE_OBJ_DRAW_SPRITE_H_
# define ESGE_OBJ_DRAW_SPRITE_H_

# include "ESGE_display.h"

# include "ESGE_objPoint.h"
# include "ESGE_objDraw.h"


/**
 * \class ESGE_ObjDrawSprite
 * \brief Represents a drawable sprite game object with position and layer-based drawing using Painter's algorithm.
 * Inherits from ESGE_ObjPoint for scene position functionality and ESGE_ObjDraw for drawing functionality.
 */
class ESGE_ObjDrawSprite:
  public virtual ESGE_ObjPoint,
  public ESGE_ObjDraw
{
public:
  ESGE_Sprite sprite; /**< The sprite to be drawn. */

  /**
   * \brief Default constructor for ESGE_ObjDrawSprite.
   */
  ESGE_ObjDrawSprite(void);
  /**
   * \brief Virtual destructor for ESGE_ObjDrawSprite.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjDrawSprite(void) = 0;
  
  /**
   * \brief Override of the OnDraw function to draw the sprite.
   */
  virtual void OnDraw(void) override;
};

#endif