#include "ESGE_objSerial.h"

#define SGLIB_ASSERT SDL_assert
#include "sglib.h"
#include "ESGE_error.h"
#include "ESGE_io.h"


void
ESGE_Field::GetValue(void *obj, char *value, size_t len) const
{
  const void *data;

  data = get(obj);

  switch (type)
  {
  case ESGE_Field::C:
    if (!SDL_snprintf(value, len, "%d", (Sint8)*(char*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::I:
    if (!SDL_snprintf(value, len, "%d", (Sint16)*(int*)data))
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::L:
    if (
      !SDL_snprintf(value, len, "%" SDL_PRIs32, (Sint32)*(long*)data)
    )
      ESGE_Error("Cannot convert field \"%s\" to string", name);
    break;
  case ESGE_Field::LL:
    if (
      SDL_snprintf(
        value,
        len,
        "%" SDL_PRIs64,
        (Sint64)*(long long*)data
      )
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
ESGE_Field::SetValue(void *obj, const char *value) const
{

  if (!SDL_strcmp(name, name))
  {
    switch (type)
    {
    case ESGE_Field::C:
      {
        char var;
        var = SDL_atoi(value);
        set(obj, &var);
      }
      break;
    case ESGE_Field::I:
      {
        int var;
        var = SDL_atoi(value);
        set(obj, &var);
      }
      break;
    case ESGE_Field::L:
      {
        long var;
        var = SDL_strtol(value, NULL, 10);
        set(obj, &var);
      }
      break;
    case ESGE_Field::LL:
      {
        long long var;
        var = SDL_strtoll(value, NULL, 10);
        set(obj, &var);
      }
      break;
    case ESGE_Field::F:
      {
        float var;
        var = SDL_atof(value);
        set(obj, &var);
      }
      break;
    case ESGE_Field::S:
      set(obj, value);
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


#define ESGE_CMP_TYPE(left, right) ((left)->id - (right)->id)

ESGE_Type::ESGE_Type(
  Uint64 id,
  const ESGE_Field *fields,
  size_t nFields
): id(id), fields(fields), nFields(nFields)
{
  SGLIB_SORTED_LIST_ADD(ESGE_Type, list, this, ESGE_CMP_TYPE, next);
}

ESGE_Type::~ESGE_Type(void)
{
  SGLIB_SORTED_LIST_DELETE(ESGE_Type, list, this, next);
}



ESGE_ObjSerial::ESGE_ObjSerial(void) {}

ESGE_ObjSerial::~ESGE_ObjSerial(void) {}


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

    tEnum = ESGE_ReadU8(io);

    for (size_t i = 0; i < t->nFields && !fMatch; ++i)
    {
      const ESGE_Field *f = t->fields + i;

      if (f->id == fieldID && tEnum == f->type)
      {
        switch (tEnum)
        {
        case ESGE_Field::C:
          {
            char var;
            var = ESGE_ReadS8(io);
            f->set(this, &var);
          }
          break;
        case ESGE_Field::I:
          {
            int var;
            var = ESGE_ReadS16(io);
            f->set(this, &var);
          }
          break;
        case ESGE_Field::L:
          {
            long var;
            var = ESGE_ReadS32(io);
            f->set(this, &var);
          }
          break;
        case ESGE_Field::LL:
          {
            long long var;
            var = ESGE_ReadS64(io);
            f->set(this, &var);
          }
          break;
        case ESGE_Field::F:
          {
            float var;
            var = ESGE_ReadFloat(io);
            f->set(this, &var);
          }
          break;
        case ESGE_Field::S:
          {
            char var[ESGE_MAX_STR];
            ESGE_ReadStr(io, var, ESGE_MAX_STR);
            f->set(this, var);
          }
          break;
        default:
          SDL_assert(!"unrecognised type");
        }
        fMatch = true;
      }
    }

    if (!fMatch)
    {
      Sint64 offset = 0;

      switch (tEnum)
      {
      case ESGE_Field::C:
      case ESGE_Field::S:
        offset = 1;
        break;
      case ESGE_Field::I:
        offset = 2;
        break;
      case ESGE_Field::L:
        offset = 4;
        break;
      case ESGE_Field::LL:
        offset = 8;
        break;
      case ESGE_Field::F:
        offset = 4;
        break;
      default:
        SDL_assert(!"unrecognised type");
      }
      if (SDL_RWseek(io, offset, RW_SEEK_CUR) == -1)
      {
        ESGE_Error(
          "Cannot seek file when loading obj with typeID=%"
          SDL_PRIu64,
          typeID
        );
      }
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
      {
        const char *data;
        data = (const char*)f->get(this);
        ESGE_WriteS8(io, *data);
      }
      break;
    case ESGE_Field::I:
      {
        const int *data;
        data = (const int*)f->get(this);
        ESGE_WriteS16(io, *data);
      }
      break;
    case ESGE_Field::L:
      {
        const long *data;
        data = (const long*)f->get(this);
        ESGE_WriteS32(io, *data);
      }
      break;
    case ESGE_Field::LL:
      {
        const long long *data;
        data = (const long long*)f->get(this);
        ESGE_WriteS64(io, *data);
      }
      break;
    case ESGE_Field::F:
      {
        const float *data;
        data = (const float*)f->get(this);
        ESGE_WriteFloat(io, *data);
      }
      break;
    case ESGE_Field::S:
      {
        const char *data;
        data = (const char*)f->get(this);
        ESGE_WriteStr(io, data, ESGE_MAX_STR);
      }
      break;
    default:
      SDL_assert(!"unrecognised type");
    }
  }

  ESGE_WriteU64(io, 0);
}
