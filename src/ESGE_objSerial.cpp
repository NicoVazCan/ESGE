#include "ESGE_objSerial.h"
#include "sglib.h"

#define ESGE_CMP_LOADER(left, right) (  \
  (left)->typeID - (right)->typeID      \
)

ESGE_ObjSerial::ESGE_ObjSerial(Uint16 id): id(id) {}

ESGE_ObjSerial::~ESGE_ObjSerial(void) { OnDisable(); }

void
ESGE_ObjSerial::OnSave(SDL_RWops *io) const
{
  if (!SDL_WriteBE16(io, id))
  {
    SDL_LogError(
      SDL_LOG_CATEGORY_SYSTEM,
      "Error %s:%s:%d: %s",
      SDL_FILE,
      SDL_FUNCTION,
      SDL_LINE,
      SDL_GetError()
    );
    SDL_ClearError();
    SDL_assert("Failed to write ObjSerial id" == NULL);
  }
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
    ESGE_CMP_LOADER,
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
