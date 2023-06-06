#include "ESGE_objSerial.h"
#include "sglib.h"


SDL_INLINE int
ESGE_CmpObjSerial(
  const ESGE_ObjSerial *left,
  const ESGE_ObjSerial *right
)
{
  return left->id - right->id;
}

ESGE_ObjSerial *ESGE_ObjSerial::list = NULL;

ESGE_ObjSerial::ESGE_ObjSerial(Uint16 id): id(id)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_ObjSerial,
    list,
    this,
    ESGE_CmpObjSerial,
    next
  );
}

ESGE_ObjSerial::~ESGE_ObjSerial(void)
{
  SGLIB_SORTED_LIST_DELETE(ESGE_ObjSerial, list, this, next);
}

ESGE_ObjSerial*
ESGE_ObjSerial::GetObj(Uint16 id)
{
  ESGE_ObjSerial *p;

  for (p = list; p != NULL && p->id < id; p = p->next);

  if (p != NULL && p->id == id) return p;
  return NULL;
}


SDL_INLINE int
ESGE_CmpLoader(const ESGE_Loader *left, const ESGE_Loader *right)
{
  return left->typeID - right->typeID;
}

ESGE_Loader *ESGE_Loader::list = NULL;

ESGE_Loader::ESGE_Loader(
  Uint16 typeID,
  ESGE_Load load
): typeID(typeID), load(load)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_Loader,
    list,
    this,
    ESGE_CmpLoader,
    next
  );
}

ESGE_Loader::~ESGE_Loader(void)
{
  SGLIB_SORTED_LIST_DELETE(ESGE_Loader, list, this, next);
}

ESGE_ObjSerial*
ESGE_Loader::Load(Uint16 typeID, SDL_RWops *io)
{
  ESGE_Loader *p;

  for (p = list; p != NULL && p->typeID < typeID; p = p->next);

  if (p != NULL && p->typeID == typeID) return p->load(io);
  return NULL;
}
