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

# define ESGE_FIELD(NAME, GET, SET, NEXT) \
ESGE_Field(#NAME, GET, SET), NEXT

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
    ESGE_FieldValue<char>                   valueC;
    ESGE_FieldValue<int>                    valueI;
    ESGE_FieldValue<long>                   valueL;
    ESGE_FieldValue<long long>              valueLL;
    ESGE_FieldValue<float>                  valueF;
    ESGE_FieldValue<char*>                  valueS;
  };

  ESGE_Field(
    const char* name,
    char (*get)(void *obj),
    void (*set)(void *obj, char value)
  );
  ESGE_Field(
    const char* name,
    int (*get)(void *obj),
    void (*set)(void *obj, int value)
  );
  ESGE_Field(
    const char *name,
    long (*get)(void *obj),
    void (*set)(void *obj, long value)
  );
  ESGE_Field(
    const char *name,
    long long (*get)(void *obj),
    void (*set)(void *obj, long long value)
  );
  ESGE_Field(
    const char *name,
    float (*get)(void *obj),
    void (*set)(void *obj, float value)
  );
  ESGE_Field(
    const char *name,
    char* (*get)(void *obj, char *str, size_t n),
    void (*set)(void *obj, const char *str)
  );

  void GetValue(void *obj, char *value, size_t len) const;
  void GetValue(void *obj, void *value, size_t len) const;

  void SetValue(void *obj, const char *value) const;
  void SetValue(void *obj, const void *value) const;
};


class ESGE_Type
{
  static ESGE_Type *list;
  ESGE_Type *next;

public:
  static const ESGE_Type *Get(Uint64 id);
  static const ESGE_Type *Get(const char *typeName);

  const Uint64 id;
  const ESGE_Field *const fields;
  const size_t nFields;

  ESGE_Type(Uint64 id, const ESGE_Field *fields, size_t nFields);
  virtual ~ESGE_Type(void);

  const ESGE_Field *GetField(Uint64 fieldId) const;
  const ESGE_Field *GetField(const char *fieldName) const;

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