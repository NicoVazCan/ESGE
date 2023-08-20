#include "ESGE_objCell.h"


const int ESGE_ObjCell::cellW = 16, ESGE_ObjCell::cellH = 16;


ESGE_ObjCell::ESGE_ObjCell(void)
{}

ESGE_ObjCell::~ESGE_ObjCell(void)
{}


int
ESGE_ObjCell::GetCol(void)
{
  return pos.x / cellW;
}

void
ESGE_ObjCell::SetCol(int col)
{
  pos.x = col * cellW;
}


int
ESGE_ObjCell::GetRow(void)
{
  return pos.y / cellH;
}

void
ESGE_ObjCell::SetRow(int row)
{
  pos.y = row * cellH;
}
