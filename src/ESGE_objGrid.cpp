#include "ESGE_objGrid.h"


const int ESGE_ObjGrid::cellW = 16, ESGE_ObjGrid::cellH = 16;


ESGE_ObjGrid::ESGE_ObjGrid(void)
{}

ESGE_ObjGrid::~ESGE_ObjGrid(void)
{}


int
ESGE_ObjGrid::GetCol(void)
{
  return pos.x / cellW;
}

void
ESGE_ObjGrid::SetCol(int col)
{
  pos.x = col * cellW;
}


int
ESGE_ObjGrid::GetRow(void)
{
  return pos.y / cellH;
}

void
ESGE_ObjGrid::SetRow(int row)
{
  pos.y = row * cellH;
}
