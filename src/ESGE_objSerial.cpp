#include "ESGE_objSerial.h"
#define SGLIB_ASSERT SDL_assert
#include "sglib.h"


ESGE_ObjSerial::ESGE_ObjSerial(void) {}

ESGE_ObjSerial::~ESGE_ObjSerial(void) {}

int
ESGE_ObjSerial::GetFieldValue(
  const ESGE_Field *field,
  char *value,
  size_t len
)
{
  Uint8 *data =((Uint8*)this)+field->offset;

  switch (field->type)
  {
  case ESGE_Field::S8:
    if (!SDL_snprintf(value, len, "%d", *(Sint8*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::U8:
    if (!SDL_snprintf(value, len, "%d", *(Uint8*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::S16:
    if (!SDL_snprintf(value, len, "%d", *(Sint16*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::U16:
    if (!SDL_snprintf(value, len, "%d", *(Uint16*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::S32:
    if (!SDL_snprintf(value, len, "%" SDL_PRIs32, *(Sint32*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::U32:
    if (!SDL_snprintf(value, len, "%" SDL_PRIu32, *(Uint32*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::S64:
    if (SDL_snprintf(value, len, "%" SDL_PRIs64, *(Sint64*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::U64:
    return (
      SDL_snprintf(value, len, "%" SDL_PRIu64, *(Uint64*)data)
    ) == 0;
  case ESGE_Field::FLOAT:
    if (SDL_snprintf(value, len, "%f", *(float*)data))
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  case ESGE_Field::STR:
    if (
      SDL_snprintf(value, len, "\"%s\"", (char*)data) !=
      2 + (int)SDL_strlen((char*)data)
    )
    {
      return SDL_SetError("Failed to get field value");
    }
    break;
  default:
    SDL_assert(!"unrecognised type");
  }
  return 0;
}

int
ESGE_ObjSerial::SetFieldValue(const char *name, const char *value)
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    if (!SDL_strcmp(fields[i].name, name))
    {
      Uint8 *data =((Uint8*)this)+fields[i].offset;

      switch (fields[i].type)
      {
      case ESGE_Field::S8:
        *((Sint8*)data) = SDL_atoi(value);
        break;
      case ESGE_Field::U8:
        *((Uint8*)data) = SDL_atoi(value);
        break;
      case ESGE_Field::S16:
        *((Sint16*)data) = SDL_atoi(value);
        break;
      case ESGE_Field::U16:
        *((Uint16*)data) = SDL_atoi(value);
        break;
      case ESGE_Field::S32:
        *((Sint32*)data) = SDL_strtol(value, NULL, 10);
        break;
      case ESGE_Field::U32:
        *((Uint32*)data) = SDL_strtoul(value, NULL, 10);
        break;
      case ESGE_Field::S64:
        *((Sint64*)data) = SDL_strtoll(value, NULL, 10);
        break;
      case ESGE_Field::U64:
        *((Uint64*)data) = SDL_strtoull(value, NULL, 10);
        break;
      case ESGE_Field::FLOAT:
        *((float*)data) = SDL_atof(value);
        break;
      case ESGE_Field::STR:
        SDL_strlcpy((char*)data, value, fields[i].strLen);
        break;
      default:
        SDL_assert(!"unrecognised type");
      }
      return 0;
    }
  }

  SDL_SetError("Field with name=%s not found", name);
  return -1;
}

int
ESGE_ObjSerial::OnLoad(SDL_RWops *io)
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    Uint8 *data =((Uint8*)this)+fields[i].offset;

    switch (fields[i].type)
    {
    case ESGE_Field::S8:
      if (ESGE_Read(io, (Sint8*)data)) return -1;
      break;
    case ESGE_Field::U8:
      if (ESGE_Read(io, (Uint8*)data)) return -1;
      break;
    case ESGE_Field::S16:
      if (ESGE_Read(io, (Sint16*)data)) return -1;
      break;
    case ESGE_Field::U16:
      if (ESGE_Read(io, (Uint16*)data)) return -1;
      break;
    case ESGE_Field::S32:
      if (ESGE_Read(io, (Sint32*)data)) return -1;
      break;
    case ESGE_Field::U32:
      if (ESGE_Read(io, (Uint32*)data)) return -1;
      break;
    case ESGE_Field::S64:
      if (ESGE_Read(io, (Sint64*)data)) return -1;
      break;
    case ESGE_Field::U64:
      if (ESGE_Read(io, (Uint64*)data)) return -1;
      break;
    case ESGE_Field::FLOAT:
      if (ESGE_Read(io, (float*)data)) return -1;
      break;
    case ESGE_Field::STR:
      if (ESGE_Read(io, (char*)data, fields[i].strLen)) return -1;
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
  return 0;
}

int
ESGE_ObjSerial::OnSave(SDL_RWops *io) const
{
  const ESGE_Field *fields;
  size_t nFields;

  fields = GetFields(&nFields);

  for (size_t i = 0; i < nFields; ++i)
  {
    Uint8 *data =((Uint8*)this)+fields[i].offset;

    switch (fields[i].type)
    {
    case ESGE_Field::S8:
      if (ESGE_Write(io, *(Sint8*)data)) return -1;
      break;
    case ESGE_Field::U8:
      if (ESGE_Write(io, *(Uint8*)data)) return -1;
      break;
    case ESGE_Field::S16:
      if (ESGE_Write(io, *(Sint16*)data)) return -1;
      break;
    case ESGE_Field::U16:
      if (ESGE_Write(io, *(Uint16*)data)) return -1;
      break;
    case ESGE_Field::S32:
      if (ESGE_Write(io, *(Sint32*)data)) return -1;
      break;
    case ESGE_Field::U32:
      if (ESGE_Write(io, *(Uint32*)data)) return -1;
      break;
    case ESGE_Field::S64:
      if (ESGE_Write(io, *(Sint64*)data)) return -1;
      break;
    case ESGE_Field::U64:
      if (ESGE_Write(io, *(Uint64*)data)) return -1;
      break;
    case ESGE_Field::FLOAT:
      if (ESGE_Write(io, *(float*)data)) return -1;
      break;
    case ESGE_Field::STR:
      if (ESGE_Write(io, (char*)data, fields[i].strLen)) return -1;
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
  return 0;
}

const ESGE_Field*
ESGE_ObjSerial::GetFields(size_t *nFields) const
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


int
ESGE_Read(SDL_RWops *io, Sint8 *ptr)
{
  SDL_assert(ptr);
  return !SDL_RWread(io, ptr, sizeof (*ptr), 1);
}

int
ESGE_Read(SDL_RWops *io, Uint8 *ptr)
{
  SDL_assert(ptr);
  return !SDL_RWread(io, ptr, sizeof (*ptr), 1);
}

int
ESGE_Read(SDL_RWops *io, Sint16 *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapBE16(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, Uint16 *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapBE16(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, Sint32 *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapBE32(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, Uint32 *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapBE32(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, Sint64 *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapBE64(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, Uint64 *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapBE64(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, float *ptr)
{
  SDL_assert(ptr);

  if (!SDL_RWread(io, ptr, sizeof (*ptr), 1)) return -1;
  *ptr = SDL_SwapFloatBE(*ptr);

  return 0;
}

int
ESGE_Read(SDL_RWops *io, char *str, size_t n)
{
  SDL_assert(str);

  for (; n > 0; --n)
  {
    if (ESGE_Read(io, (Uint8*)str)) return -1;
    else if (*str++ == '\0') break;
  }
  for (; n > 0; --n) *str++ = '\0';

  return 0;
}


int
ESGE_Write(SDL_RWops *io, Sint8 value)
{
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Uint8 value)
{
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Sint16 value)
{
  value = SDL_SwapBE16(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Uint16 value)
{
  value = SDL_SwapBE16(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Sint32 value)
{
  value = SDL_SwapBE32(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Uint32 value)
{
  value = SDL_SwapBE32(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Sint64 value)
{
  value = SDL_SwapBE64(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, Uint64 value)
{
  value = SDL_SwapBE64(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

int
ESGE_Write(SDL_RWops *io, float value)
{
  value = SDL_SwapFloatBE(value);
  if (!SDL_RWwrite(io, &value, sizeof (value), 1)) return -1;
  return 0;
}

void
ESGE_Write(SDL_RWops *io, const char *str, size_t n)
{
  SDL_assert(str);

  for (const char *c = str; n > 0; --n)
  {
    if (ESGE_Write(io, *(Uint8*)c)) return -1;
    else if (*c++ == '\0') break;
  }
  return 0;
}
