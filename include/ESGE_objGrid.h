#ifndef ESGE_OBJ_GRID_H_
# define ESGE_OBJ_GRID_H_

# include "ESGE_objPoint.h"


class ESGE_ObjGrid: public virtual ESGE_ObjPoint
{
public:
  static const int cellW, cellH;

  ESGE_ObjGrid(void);
  virtual ~ESGE_ObjGrid(void) = 0;
  
  int GetCol(void);
  void SetCol(int col);

  int GetRow(void);
  void SetRow(int row);
};

#endif