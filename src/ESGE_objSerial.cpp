#include "ESGE_objSerial.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_error.h"
#include "ESGE_hash.h"
#include "ESGE_io.h"

void
ESGE_Field::GetValue(char *value, size_t len) const
{
  switch (type)
  {
  case ESGE_Field::C:
    if (!SDL_snprintf(value, len, "%d", *(char*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::I:
    if (!SDL_snprintf(value, len, "%d", *(int*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::L:
    if (
      !SDL_snprintf(value, len, "%" SDL_PRIs32, *(long*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::LL:
    if (
      SDL_snprintf(value, len, "%" SDL_PRIs64, *(long long*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::F:
    if (SDL_snprintf(value, len, "%f", *(float*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::S:
    if (
      SDL_snprintf(value, len, "\"%s\"", (char*)data) !=
      2 + (int)SDL_strlen((char*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  default:
    SDL_assert(!"unrecognised type");
  }
}

void
ESGE_Field::SetValue(const char *value) const
{
  if (!SDL_strcmp(name, name))
  {
    switch (type)
    {
    case ESGE_Field::C:
      *(char*)data = SDL_atoi(value);
      break;
    case ESGE_Field::I:
      *(int*)data = SDL_atoi(value);
      break;
    case ESGE_Field::L:
      *(long*)data = SDL_strtol(value, NULL, 10);
      break;
    case ESGE_Field::LL:
      *(long long*)data = SDL_strtoll(value, NULL, 10);
      break;
    case ESGE_Field::F:
      *(float*)data = SDL_atof(value);
      break;
    case ESGE_Field::S:
      SDL_strlcpy((char*)data, value, strLen);
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}


ESGE_ObjSerial::ESGE_ObjSerial(void) {}

ESGE_ObjSerial::~ESGE_ObjSerial(void) {}


void
ESGE_ObjSerial::Load(SDL_RWops *io)
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    switch (fields[i].type)
    {
    case ESGE_Field::C:
      *(char*)fields[i].data = ESGE_ReadS8(io);
      break;
    case ESGE_Field::I:
      *(int*)fields[i].data = ESGE_ReadS16(io);
      break;
    case ESGE_Field::L:
      *(long*)fields[i].data = ESGE_ReadS32(io);
      break;
    case ESGE_Field::LL:
      *(long long*)fields[i].data = ESGE_ReadS64(io);
      break;
    case ESGE_Field::F:
      *(float*)fields[i].data = ESGE_ReadFloat(io);
      break;
    case ESGE_Field::S:
      ESGE_ReadStr(io, (char*)fields[i].data, fields[i].strLen);
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}

void
ESGE_ObjSerial::Save(SDL_RWops *io)
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    switch (fields[i].type)
    {
    case ESGE_Field::C:
      ESGE_WriteS8(io, *(char*)fields[i].data);
      break;
    case ESGE_Field::I:
      ESGE_WriteS16(io, *(int*)fields[i].data);
      break;
    case ESGE_Field::L:
      ESGE_WriteS32(io, *(long*)fields[i].data);
      break;
    case ESGE_Field::LL:
      ESGE_WriteS64(io, *(Sint64*)fields[i].data);
      break;
    case ESGE_Field::F:
      ESGE_WriteFloat(io, *(float*)fields[i].data);
      break;
    case ESGE_Field::S:
      ESGE_WriteStr(io, (char*)fields[i].data, fields[i].strLen);
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}

const ESGE_Field*
ESGE_ObjSerial::GetFields(size_t *nFields)
{
  *nFields = 0;
  return NULL;
}


#define ESGE_CMP_TYPE(left, right) ((left)->typeID - (right)->typeID)

ESGE_Type *ESGE_Type::list = NULL;

ESGE_Type::ESGE_Type(
  const char *typeName,
  ESGE_NewCall newObj
):
  typeID(ESGE_Hash(typeName)),
  newObj(newObj)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_Type,
    list,
    this,
    ESGE_CMP_TYPE,
    next
  );
}

ESGE_Type::~ESGE_Type(void)
{
  SGLIB_SORTED_LIST_DELETE(ESGE_Type, list, this, next);
}

ESGE_ObjSerial*
ESGE_Type::New(Uint64 typeID)
{
  ESGE_Type *p;

  for (p = list; p != NULL && p->typeID < typeID; p = p->next);

  if (p != NULL && p->typeID == typeID)
  {
    ESGE_ObjSerial *obj = p->newObj();
    obj->typeID = typeID;

    return obj;
  }

  SDL_SetError(
    "Object type with typeID=%" SDL_PRIu64 " not found",
    typeID
  );
  return NULL;
}

ESGE_ObjSerial*
ESGE_Type::New(const char *typeName)
{
  return New(ESGE_Hash(typeName));
}
