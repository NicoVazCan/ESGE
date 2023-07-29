#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"
# include "ESGE_mem.h"

# define ESGE_ADD_TYPE(type) \
static ESGE_TypeImpl<type> ESGE__type##type(#type);

#ifndef offsetof
#define offsetof(type, field) ((size_t) &(((type*)0)->field))
#endif

struct ESGE_Field
{
  enum
  {
    S8,  U8,  S16, U16,  S32, U32,  S64, U64,  FLOAT,  STR
  } type;
  const char *name;
  void *data;
  size_t strLen;

  void GetValue(char *value, size_t len) const;
  void SetValue(const char *value) const;
};

class ESGE_ObjSerial
{
public:
  Uint64 typeID = 0;

  ESGE_ObjSerial(void);
  virtual ~ESGE_ObjSerial(void) = 0;

  virtual const ESGE_Field *GetFields(size_t *nFields);

  void OnLoad(SDL_RWops *io);
  void OnSave(SDL_RWops *io);
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
    return new C;
  }

public:
  ESGE_TypeImpl(const char *typeName):
    ESGE_Type(typeName, New)
  {}
};

Sint8  ESGE_ReadS8(   SDL_RWops *io);
Uint8  ESGE_ReadU8(   SDL_RWops *io);
Sint16 ESGE_ReadS16(  SDL_RWops *io);
Uint16 ESGE_ReadU16(  SDL_RWops *io);
Sint32 ESGE_ReadS32(  SDL_RWops *io);
Uint32 ESGE_ReadU32(  SDL_RWops *io);
Sint64 ESGE_ReadS64(  SDL_RWops *io);
Uint64 ESGE_ReadU64(  SDL_RWops *io);
float  ESGE_ReadFloat(SDL_RWops *io);
char*  ESGE_ReadStr(  SDL_RWops *io, char *str, size_t n);

void ESGE_WriteS8(   SDL_RWops *io, Sint8  value);
void ESGE_WriteU8(   SDL_RWops *io, Uint8  value);
void ESGE_WriteS16(  SDL_RWops *io, Sint16 value);
void ESGE_WriteU16(  SDL_RWops *io, Uint16 value);
void ESGE_WriteS32(  SDL_RWops *io, Sint32 value);
void ESGE_WriteU32(  SDL_RWops *io, Uint32 value);
void ESGE_WriteS64(  SDL_RWops *io, Sint64 value);
void ESGE_WriteU64(  SDL_RWops *io, Uint64 value);
void ESGE_WriteFloat(SDL_RWops *io, float  value);
void ESGE_WriteStr(  SDL_RWops *io, const char *str, size_t n);

#endif