#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"


# define ESGE_NO_FIELD

# define ESGE_FIELD(TYPE_ID, NAME, GET, SET, NEXT) \
{ ESGE_Field::TYPE_ID, ESGE_Hash(#NAME), #NAME, GET, SET }, NEXT

# define ESGE_TYPE(CLASS, FIELDS) \
static const ESGE_TypeImpl<CLASS, ESGE_Hash(#CLASS)> \
ESGE__type##CLASS; \
template<> \
const ESGE_Field ESGE_TypeImpl<CLASS, ESGE_Hash(#CLASS)>::fields[] = \
{ FIELDS };


# define ESGE_MAX_STR 256

typedef const void *(*ESGE_FieldGetter)(void *obj);
typedef void (*ESGE_FieldSetter)(void *obj, const void *data);

struct ESGE_Field
{
  const enum {C,I,L,LL,F,S} type;
  const Uint64 id;
  const char *name;
  const ESGE_FieldGetter get;
  const ESGE_FieldSetter set;

  void GetValue(void *obj, char *value, size_t len) const;
  void SetValue(void *obj, const char *value) const;
};


class ESGE_Type
{
  static ESGE_Type *list;

  ESGE_Type *next;

public:
  const Uint64 id;
  const ESGE_Field *const fields;
  const size_t nFields;

  static const ESGE_Type *Get(Uint64 id);

  ESGE_Type(Uint64 id, const ESGE_Field *fields, size_t nFields);
  virtual ~ESGE_Type(void);

  virtual void *New(void) const = 0;
};

template<class C, Uint64 ID>
class ESGE_TypeImpl: ESGE_Type
{
  virtual void *New(void) const
  {
    return new C;
  }

public:
  static const ESGE_Field fields[];

  ESGE_TypeImpl(void):
    ESGE_Type(ID, fields, sizeof(fields) / sizeof(ESGE_Field))
  {}
  virtual ~ESGE_TypeImpl(void)
  {}
};


class ESGE_ObjSerial
{
public:
  Uint64 typeID = 0;

  ESGE_ObjSerial(void);
  virtual ~ESGE_ObjSerial(void) = 0;

  void Load(SDL_RWops *io);
  void Save(SDL_RWops *io);
};

#endif