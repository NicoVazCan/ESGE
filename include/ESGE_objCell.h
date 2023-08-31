/**
 * \file ESGE_objCell.h
 * \brief Contains the definition of the ESGE_ObjCell class which represents a game object which position is fixed by a grid.
 */
#ifndef ESGE_OBJ_CELL_H_
# define ESGE_OBJ_CELL_H_

# include "ESGE_objPoint.h"


/**
 * \class ESGE_ObjCell
 * \brief Represents a game object which position is fixed by a grid.
 * Inherits from ESGE_ObjPoint for scene position functionality.
 */
class ESGE_ObjCell: public virtual ESGE_ObjPoint
{
public:
  static const int cellW; /**< The width of the cell. */
  static const int cellH; /**< The height of the cell. */

  /**
   * \brief Default constructor for ESGE_ObjCell.
   */
  ESGE_ObjCell(void);
  /**
   * \brief Virtual destructor for ESGE_ObjCell.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjCell(void) = 0;
  
  /**
   * \brief Get the column index of the cell based on game object position.
   * \return The column index of the cell.
   */
  int GetCol(void);
  /**
   * \brief Set game object position based on the column index of the cell.
   * \param col The new column index to set.
   */
  void SetCol(int col);

  /**
   * \brief Get the row index of the cell based on game object position.
   * \return The row index of the cell.
   */
  int GetRow(void);
  /**
   * \brief Set game object position based on the row index of the cell.
   * \param row The new row index to set.
   */
  void SetRow(int row);
};

#endif