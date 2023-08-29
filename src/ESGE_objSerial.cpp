#include "ESGE_objSerial.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_error.h"
#include "ESGE_io.h"


ESGE_Field::ESGE_Field(
  const char* name,
  char (*get)(void *obj),
  void (*set)(void *obj, char value)
):
  type(C),
  id(ESGE_Hash(name)),
  name(name),
  valueC{get, set}
{}
ESGE_Field::ESGE_Field(
  const char* name,
  int (*get)(void *obj),
  void (*set)(void *obj, int value)
):
  type(I),
  id(ESGE_Hash(name)),
  name(name),
  valueI{get, set}
{}
ESGE_Field::ESGE_Field(
  const char *name,
  long (*get)(void *obj),
  void (*set)(void *obj, long value)
):
  type(L),
  id(ESGE_Hash(name)),
  name(name),
  valueL{get, set}
{}
ESGE_Field::ESGE_Field(
  const char *name,
  long long (*get)(void *obj),
  void (*set)(void *obj, long long value)
):
  type(L),
  id(ESGE_Hash(name)),
  name(name),
  valueLL{get, set}
{}
ESGE_Field::ESGE_Field(
  const char *name,
  float (*get)(void *obj),
  void (*set)(void *obj, float value)
):
  type(F),
  id(ESGE_Hash(name)),
  name(name),
  valueF{get, set}
{}
ESGE_Field::ESGE_Field(
  const char *name,
  char* (*get)(void *obj, char *str, size_t n),
  void (*set)(void *obj, const char *str)
):
  type(S),
  id(ESGE_Hash(name)),
  name(name),
  valueS{get, set}
{}

void
ESGE_Field::GetValue(void *obj, char *value, size_t len) const
{
  switch (type)
  {
  case ESGE_Field::C:
    if (SDL_snprintf(value, len, "%d", (Sint8)valueC.get(obj)) == -1)
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::I:
    if (SDL_snprintf(value, len, "%d", (Sint16)valueI.get(obj)) == -1)
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::L:
    if (
      SDL_snprintf(
        value,
        len,
        "%" SDL_PRIs32,
        (Sint32)valueL.get(obj)
      ) == -1
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::LL:
    if (
      SDL_snprintf(
        value,
        len,
        "%" SDL_PRIs64,
        (Sint64)valueLL.get(obj)
      ) == -1
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::F:
    if (SDL_snprintf(value, len, "%f", valueF.get(obj)) == -1)
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::S:
    size_t n;

    valueS.get(obj, value+1, len-2);

    n = SDL_strlen(value+1);
    value[0] = value[n+1] = '\"';
    value[n+2] = '\0';
    break;
  default:
    SDL_assert(!"unrecognised type");
  }
}
void
ESGE_Field::GetValue(void *obj, void *value, size_t len) const
{
  switch (type)
  {
  case ESGE_Field::C:
    *(char*)value = valueC.get(obj);
    break;
  case ESGE_Field::I:
    *(int*)value = valueI.get(obj);
    break;
  case ESGE_Field::L:
    *(long*)value = valueL.get(obj);
    break;
  case ESGE_Field::LL:
    *(long long*)value = valueLL.get(obj);
    break;
  case ESGE_Field::F:
    *(float*)value = valueF.get(obj);
    break;
  case ESGE_Field::S:
    valueS.get(obj, (char*)value, len);
    break;
  default:
    SDL_assert(!"unrecognised type");
  }
}

void
ESGE_Field::SetValue(void *obj, const char *value) const
{

  if (!SDL_strcmp(name, name))
  {
    switch (type)
    {
    case ESGE_Field::C:
      valueC.set(obj, SDL_atoi(value));
      break;
    case ESGE_Field::I:
      valueI.set(obj, SDL_atoi(value));
      break;
    case ESGE_Field::L:
      valueL.set(obj, SDL_strtol(value, NULL, 10));
      break;
    case ESGE_Field::LL:
      valueLL.set(obj, SDL_strtoll(value, NULL, 10));
      break;
    case ESGE_Field::F:
      valueF.set(obj, SDL_atof(value));
      break;
    case ESGE_Field::S:
      valueS.set(obj, value);
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}
void
ESGE_Field::SetValue(void *obj, const void *value) const
{

  if (!SDL_strcmp(name, name))
  {
    switch (type)
    {
    case ESGE_Field::C:
      valueC.set(obj, *(char*)value);
      break;
    case ESGE_Field::I:
      valueI.set(obj, *(int*)value);
      break;
    case ESGE_Field::L:
      valueL.set(obj, *(long*)value);
      break;
    case ESGE_Field::LL:
      valueLL.set(obj, *(long long*)value);
      break;
    case ESGE_Field::F:
      valueF.set(obj, *(float*)value);
      break;
    case ESGE_Field::S:
      valueS.set(obj, (char*)value);
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}



ESGE_Type *ESGE_Type::list = NULL;

const ESGE_Type*
ESGE_Type::Get(Uint64 id)
{
  ESGE_Type *t;

  for (t = list; t != NULL && t->id < id; t = t->next);

  if (t != NULL && t->id == id) return t;

  return NULL;
}
const ESGE_Type*
ESGE_Type::Get(const char *typeName)
{
  return Get(ESGE_Hash(typeName));
}

ESGE_Type::ESGE_Type(
  Uint64 id,
  const ESGE_Field *fields,
  size_t nFields
): id(id), fields(fields), nFields(nFields)
{
  ESGE_Type **node;;

  for (
    node = &list;
    *node && (*node)->id < id;
    node = &(*node)->next
  );
  SDL_assert(!*node || (*node)->id != id);
  next = *node;
  *node = this;
}
ESGE_Type::~ESGE_Type(void)
{
  ESGE_Type **node;;

  for (
    node = &list;
    *node && (*node)->id < id;
    node = &(*node)->next
  );
  SDL_assert(*node && (*node)->id == id);
  *node = next;
}

const ESGE_Field*
ESGE_Type::GetField(Uint64 fieldID) const
{
  for (size_t i = 0; i < nFields; ++i)
  {
    if (fields[i].id == fieldID)
      return fields + i;
  }
  return NULL;
}
const ESGE_Field*
ESGE_Type::GetField(const char *fieldName) const
{
  return GetField(ESGE_Hash(fieldName));
}


ESGE_ObjSerial::ESGE_ObjSerial(void)
{}
ESGE_ObjSerial::~ESGE_ObjSerial(void)
{}

void
ESGE_ObjSerial::Load(SDL_RWops *io)
{
  Uint64 fieldID;
  const ESGE_Type *t;

  t = ESGE_Type::Get(typeID);

  while ((fieldID = ESGE_ReadU64(io)))
  {
    bool fMatch = false;
    Uint8 tEnum;
    const ESGE_Field *f = NULL;

    tEnum = ESGE_ReadU8(io);

    for (size_t i = 0; i < t->nFields && !fMatch; ++i)
    {
      f = t->fields + i;
      fMatch = f->id == fieldID && tEnum == f->type;
    }

    switch (tEnum)
    {
    case ESGE_Field::C:
      if (fMatch && f)
      {
        f->valueC.set(this, ESGE_ReadS8(io));
      }
      else
      {
        ESGE_ReadS8(io);
      }
      break;
    case ESGE_Field::I:
      if (fMatch && f)
      {
        f->valueI.set(this, ESGE_ReadS16(io));
      }
      else
      {
        ESGE_ReadS16(io);
      }
      break;
    case ESGE_Field::L:
      if (fMatch && f)
      {
        f->valueL.set(this, ESGE_ReadS32(io));
      }
      else
      {
        ESGE_ReadS32(io);
      }
      break;
    case ESGE_Field::LL:
      if (fMatch && f)
      {
        f->valueLL.set(this, ESGE_ReadS64(io));
      }
      else
      {
        ESGE_ReadS64(io);
      }
      break;
    case ESGE_Field::F:
      if (fMatch && f)
      {
        f->valueF.set(this, ESGE_ReadFloat(io));
      }
      else
      {
        ESGE_ReadFloat(io);
      }
      break;
    case ESGE_Field::S:
      {
        char str[ESGE_MAX_STR];

        ESGE_ReadStr(io, str, ESGE_MAX_STR);
        if (fMatch && f)
        {
          f->valueS.set(this, str);
        }
      }
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }
}

void
ESGE_ObjSerial::Save(SDL_RWops *io)
{
  const ESGE_Type *t;

  t = ESGE_Type::Get(typeID);

  for (size_t i = 0; i < t->nFields; ++i)
  {
    const ESGE_Field *f = t->fields + i;

    ESGE_WriteU64(io, f->id);
    ESGE_WriteU8(io, f->type);

    switch (f->type)
    {
    case ESGE_Field::C:
      ESGE_WriteS8(io, f->valueC.get(this));
      break;
    case ESGE_Field::I:
      ESGE_WriteS16(io, f->valueI.get(this));
      break;
    case ESGE_Field::L:
      ESGE_WriteS32(io, f->valueL.get(this));
      break;
    case ESGE_Field::LL:
      ESGE_WriteS64(io, f->valueLL.get(this));
      break;
    case ESGE_Field::F:
      ESGE_WriteFloat(io, f->valueF.get(this));
      break;
    case ESGE_Field::S:
      char str[ESGE_MAX_STR];
      ESGE_WriteStr(io, f->valueS.get(this, str, ESGE_MAX_STR));
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }

  ESGE_WriteU64(io, 0);
}
