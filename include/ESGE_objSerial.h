#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"
# include "ESGE_mem.h"

# define ESGE_ADD_TYPE(type) \
static ESGE_TypeImpl<type> ESGE__type##type(#type);

# define OFFSETOF(type, field) ((size_t) &(((type*)0)->field))

struct ESGE_Field
{
  enum
  {
    S8,  U8,  S16, U16,  S32, U32,  S64, U64,  FLOAT,  STR
  } type;
  const char *name;
  size_t offset, strLen;
};

class ESGE_ObjSerial
{
public:
  Uint64 typeID = 0;

  ESGE_ObjSerial(void);
  virtual ~ESGE_ObjSerial(void) = 0;

  virtual const ESGE_Field *GetFields(size_t *nFields) const;
  int GetFieldValue(const ESGE_Field *field, char *value, size_t len);
  int SetFieldValue(const char *name, const char *value);

  virtual int OnLoad(SDL_RWops *io);
  virtual int OnSave(SDL_RWops *io) const;
};

typedef ESGE_ObjSerial *(*ESGE_NewCall)(void);

class ESGE_Type
{
  static ESGE_Type *list;
  const Uint64 typeID;
  const ESGE_NewCall newObj;
  ESGE_Type *next;
  
protected:
  ESGE_Type(
    const char *typeName,
    ESGE_NewCall newObj
  );
  ~ESGE_Type(void);

public:
  static ESGE_ObjSerial* New(Uint64 typeID);
  static ESGE_ObjSerial* New(const char *typeName);
};

template<class C>
class ESGE_TypeImpl: ESGE_Type
{
  static ESGE_ObjSerial* New(void)
  {
    void *ptr;

    if(!(ptr = SDL_malloc(sizeof(C))))
    {
      SDL_OutOfMemory();
      return NULL;
    }
    return new(ptr) C;
  }

public:
  ESGE_TypeImpl(const char *typeName):
    ESGE_Type(typeName, New)
  {}
};

int ESGE_Read(SDL_RWops *io, Sint8  *ptr);
int ESGE_Read(SDL_RWops *io, Uint8  *ptr);
int ESGE_Read(SDL_RWops *io, Sint16 *ptr);
int ESGE_Read(SDL_RWops *io, Uint16 *ptr);
int ESGE_Read(SDL_RWops *io, Sint32 *ptr);
int ESGE_Read(SDL_RWops *io, Uint32 *ptr);
int ESGE_Read(SDL_RWops *io, Sint64 *ptr);
int ESGE_Read(SDL_RWops *io, Uint64 *ptr);
int ESGE_Read(SDL_RWops *io, float  *ptr);
int ESGE_Read(SDL_RWops *io, char   *str, size_t n);

int ESGE_Write(SDL_RWops *io, Sint8       value);
int ESGE_Write(SDL_RWops *io, Uint8       value);
int ESGE_Write(SDL_RWops *io, Sint16      value);
int ESGE_Write(SDL_RWops *io, Uint16      value);
int ESGE_Write(SDL_RWops *io, Sint32      value);
int ESGE_Write(SDL_RWops *io, Uint32      value);
int ESGE_Write(SDL_RWops *io, Sint64      value);
int ESGE_Write(SDL_RWops *io, Uint64      value);
int ESGE_Write(SDL_RWops *io, float       value);
int ESGE_Write(SDL_RWops *io, const char *str, size_t n);

#endif