#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>

# define ESGE_ADD_TYPE(type) \
static ESGE_TypeImpl<type> ESGE__type##type(#type);

struct ESGE_Field
{
  enum {C,I,L,LL,F,S} type;
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

  void Load(SDL_RWops *io);
  void Save(SDL_RWops *io);
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

#endif