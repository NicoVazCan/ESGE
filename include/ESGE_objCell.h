#ifndef ESGE_OBJ_CELL_H_
# define ESGE_OBJ_CELL_H_

# include "ESGE_objPoint.h"


class ESGE_ObjCell: public virtual ESGE_ObjPoint
{
public:
  static const int cellW, cellH;

  ESGE_ObjCell(void);
  virtual ~ESGE_ObjCell(void) = 0;
  
  int GetCol(void);
  void SetCol(int col);

  int GetRow(void);
  void SetRow(int row);
};

#endif