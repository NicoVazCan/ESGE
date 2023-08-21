#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"


# define ESGE_TYPE(CLASS) \
static const ESGE_TypeImpl<CLASS> ESGE__type##CLASS( \
  ESGE_Hash(#CLASS), \
  NULL, \
  0 \
);

# define ESGE_END_FIELD

# define ESGE_FIELD(TYPE_ID, NAME, GET, SET, NEXT) \
{ \
  ESGE_Field::TYPE_ID, \
  ESGE_Hash(#NAME), \
  #NAME, \
  { (void*)GET, (void*)SET } \
}, NEXT

# define ESGE_TYPE_FIELDS(CLASS, FIELDS) \
static const ESGE_Field ESGE__fields##CLASS[] = \
{ FIELDS }; \
static const ESGE_TypeImpl<CLASS> ESGE__type##CLASS( \
  ESGE_Hash(#CLASS), \
  ESGE__fields##CLASS, \
  SDL_arraysize(ESGE__fields##CLASS) \
);



# define ESGE_MAX_STR 256

template<typename T>
struct ESGE_FieldValue
{
  T    (*const get)(void *obj);
  void (*const set)(void *obj, T value);
};

template<>
struct ESGE_FieldValue<char*>
{
  char* (*const get)(void *obj, char* str, size_t n);
  void  (*const set)(void *obj, const char* str);
};


struct ESGE_Field
{
  const enum {C,I,L,LL,F,S} type;
  const Uint64 id;
  const char *name;
  union
  {
    struct { void *const get, *const set; } value;
    ESGE_FieldValue<char>                   valueC;
    ESGE_FieldValue<int>                    valueI;
    ESGE_FieldValue<long>                   valueL;
    ESGE_FieldValue<long long>              valueLL;
    ESGE_FieldValue<float>                  valueF;
    ESGE_FieldValue<char*>                  valueS;
  };

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

template<class C>
class ESGE_TypeImpl: ESGE_Type
{
  virtual void *New(void) const
  {
    return new C;
  }

public:
  ESGE_TypeImpl(Uint64 id, const ESGE_Field *fields, size_t nFields):
    ESGE_Type(id, fields, nFields)
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