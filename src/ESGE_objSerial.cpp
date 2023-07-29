#include "ESGE_objSerial.h"
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_error.h"


void
ESGE_Field::GetValue(char *value, size_t len) const
{
  switch (type)
  {
  case ESGE_Field::S8:
    if (!SDL_snprintf(value, len, "%d", *(Sint8*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::U8:
    if (!SDL_snprintf(value, len, "%d", *(Uint8*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::S16:
    if (!SDL_snprintf(value, len, "%d", *(Sint16*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::U16:
    if (!SDL_snprintf(value, len, "%d", *(Uint16*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::S32:
    if (
      !SDL_snprintf(value, len, "%" SDL_PRIs32, *(Sint32*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::U32:
    if (
      !SDL_snprintf(value, len, "%" SDL_PRIu32, *(Uint32*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::S64:
    if (
      SDL_snprintf(value, len, "%" SDL_PRIs64, *(Sint64*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::U64:
    if (
      (SDL_snprintf(value, len, "%" SDL_PRIu64, *(Uint64*)data)) == 0
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::FLOAT:
    if (SDL_snprintf(value, len, "%f", *(float*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::STR:
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
    case ESGE_Field::S8:
      *(Sint8*)data = SDL_atoi(value);
      break;
    case ESGE_Field::U8:
      *(Uint8*)data = SDL_atoi(value);
      break;
    case ESGE_Field::S16:
      *(Sint16*)data = SDL_atoi(value);
      break;
    case ESGE_Field::U16:
      *(Uint16*)data = SDL_atoi(value);
      break;
    case ESGE_Field::S32:
      *(Sint32*)data = SDL_strtol(value, NULL, 10);
      break;
    case ESGE_Field::U32:
      *(Uint32*)data = SDL_strtoul(value, NULL, 10);
      break;
    case ESGE_Field::S64:
      *(Sint64*)data = SDL_strtoll(value, NULL, 10);
      break;
    case ESGE_Field::U64:
      *(Uint64*)data = SDL_strtoull(value, NULL, 10);
      break;
    case ESGE_Field::FLOAT:
      *(float*)data = SDL_atof(value);
      break;
    case ESGE_Field::STR:
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
ESGE_ObjSerial::OnLoad(SDL_RWops *io)
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    switch (fields[i].type)
    {
    case ESGE_Field::S8:
      *(Sint8*)fields[i].data = ESGE_ReadS8(io);
      break;
    case ESGE_Field::U8:
      *(Uint8*)fields[i].data = ESGE_ReadU8(io);
      break;
    case ESGE_Field::S16:
      *(Sint16*)fields[i].data = ESGE_ReadS16(io);
      break;
    case ESGE_Field::U16:
      *(Uint16*)fields[i].data = ESGE_ReadU16(io);
      break;
    case ESGE_Field::S32:
      *(Sint32*)fields[i].data = ESGE_ReadS32(io);
      break;
    case ESGE_Field::U32:
      *(Uint32*)fields[i].data = ESGE_ReadU32(io);
      break;
    case ESGE_Field::S64:
      *(Sint64*)fields[i].data = ESGE_ReadS64(io);
      break;
    case ESGE_Field::U64:
      *(Uint64*)fields[i].data = ESGE_ReadU64(io);
      break;
    case ESGE_Field::FLOAT:
      *(float*)fields[i].data = ESGE_ReadFloat(io);
      break;
    case ESGE_Field::STR:
      ESGE_ReadStr(io, (char*)fields[i].data, fields[i].strLen);
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}

void
ESGE_ObjSerial::OnSave(SDL_RWops *io)
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    switch (fields[i].type)
    {
    case ESGE_Field::S8:
      ESGE_WriteS8(io, *(Sint8*)fields[i].data);
      break;
    case ESGE_Field::U8:
      ESGE_WriteU8(io, *(Uint8*)fields[i].data);
      break;
    case ESGE_Field::S16:
      ESGE_WriteS16(io, *(Sint16*)fields[i].data);
      break;
    case ESGE_Field::U16:
      ESGE_WriteU16(io, *(Uint16*)fields[i].data);
      break;
    case ESGE_Field::S32:
      ESGE_WriteS32(io, *(Sint32*)fields[i].data);
      break;
    case ESGE_Field::U32:
      ESGE_WriteU32(io, *(Uint32*)fields[i].data);
      break;
    case ESGE_Field::S64:
      ESGE_WriteS64(io, *(Sint64*)fields[i].data);
      break;
    case ESGE_Field::U64:
      ESGE_WriteU64(io, *(Uint64*)fields[i].data);
      break;
    case ESGE_Field::FLOAT:
      ESGE_WriteFloat(io, *(float*)fields[i].data);
      break;
    case ESGE_Field::STR:
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


Sint8
ESGE_ReadS8(SDL_RWops *io)
{
  Sint8 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return value;
}

Uint8
ESGE_ReadU8(SDL_RWops *io)
{
  Uint8 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return value;
}

Sint16
ESGE_ReadS16(SDL_RWops *io)
{
  Sint16 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE16(value);
}

Uint16
ESGE_ReadU16(SDL_RWops *io)
{
  Uint16 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE16(value);
}

Sint32
ESGE_ReadS32(SDL_RWops *io)
{
  Sint32 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE32(value);
}

Uint32
ESGE_ReadU32(SDL_RWops *io)
{
  Uint32 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE32(value);
}

Sint64
ESGE_ReadS64(SDL_RWops *io)
{
  Sint64 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE64(value);
}

Uint64
ESGE_ReadU64(SDL_RWops *io)
{
  Uint64 value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapBE64(value);
}

float
ESGE_ReadFloat(SDL_RWops *io)
{
  float value;

  if (!SDL_RWread(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot read: %s", SDL_GetError());

  return SDL_SwapFloatBE(value);
}

char*
ESGE_ReadStr(SDL_RWops *io, char *str, size_t n)
{
  char *c;

  SDL_assert(str);
  c = str;
  for (; n > 0 && (*c++ = ESGE_ReadU8(io)) != '\0'; --n);
  for (; n > 0; --n) *c++ = '\0';

  return str;
}


void
ESGE_WriteS8(SDL_RWops *io, Sint8 value)
{
  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU8(SDL_RWops *io, Uint8 value)
{
  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteS16(SDL_RWops *io, Sint16 value)
{
  value = SDL_SwapBE16(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU16(SDL_RWops *io, Uint16 value)
{
  value = SDL_SwapBE16(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteS32(SDL_RWops *io, Sint32 value)
{
  value = SDL_SwapBE32(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU32(SDL_RWops *io, Uint32 value)
{
  value = SDL_SwapBE32(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteS64(SDL_RWops *io, Sint64 value)
{
  value = SDL_SwapBE64(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteU64(SDL_RWops *io, Uint64 value)
{
  value = SDL_SwapBE64(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteFloat(SDL_RWops *io, float value)
{
  value = SDL_SwapFloatBE(value);

  if (!SDL_RWwrite(io, &value, sizeof (value), 1))
    ESGE_Error("Cannot write: %s", SDL_GetError());
}

void
ESGE_WriteStr(SDL_RWops *io, const char *str, size_t n)
{
  SDL_assert(str);

  for (const char *c = str; n > 0; --n)
  {
    ESGE_WriteU8(io, *(Uint8*)c);
    if (*c++ == '\0') return;
  }
}
