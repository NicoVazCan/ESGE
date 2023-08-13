//g++ main.cpp `pkg-config --cflags --libs sdl2` -o main
#include <SDL2/SDL.h>
#include "sglib.h"


typedef const void *(*ESGE_FieldGetter)(void *self);
typedef void (*ESGE_FieldSetter)(void *self, const void *data);

struct ESGE_Field
{
  const enum {C,I,L,LL,F,S} type;
  const char *name;
  const ESGE_FieldGetter get;
  const ESGE_FieldSetter set;
};


SDL_FORCE_INLINE constexpr Uint64
ESGE_Hash(const char *str)
{
  Uint64 hash = 5381;
  int c = 0;

  while ((c = *str++)) hash = ((hash << 5) + hash) + c;

  return hash;
}


class ESGE_Type
{
  static ESGE_Type *list;

  const Uint64 id;
  ESGE_Type *next;

public:
  static const ESGE_Type *Get(Uint64 id);

  ESGE_Type(Uint64 id);
  virtual ~ESGE_Type(void);

  virtual void *New(void) const = 0;
};



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

ESGE_Type::ESGE_Type(Uint64 id): id(id)
{
  SGLIB_SORTED_LIST_ADD(ESGE_Type, list, this, ESGE_CMP_TYPE, next);
}

ESGE_Type::~ESGE_Type(void)
{
  SGLIB_SORTED_LIST_DELETE(ESGE_Type, list, this, next);
}



template<class C, Uint64 ID>
class ESGE_TypeImpl: ESGE_Type
{
  virtual void *New(void) const
  {
    return new C;
  }

public:
  static ESGE_Field fields[];

  ESGE_TypeImpl(void): ESGE_Type(ID)
  {}
  virtual ~ESGE_TypeImpl(void)
  {}
};


class Test
{
public:
  Test(void) {puts(__func__);}
  ~Test(void) {puts(__func__);}
};


static const ESGE_TypeImpl<Test, 0> ESGE_type;

template<>
ESGE_Field ESGE_TypeImpl<Test, 0>::fields[] = {
  {ESGE_Field::I, "I", NULL, NULL}
};

int main(int argc, char const *argv[])
{
  const ESGE_Type *type;
  Test *test;

  type = ESGE_Type::Get(0);

  test = (Test*)type->New();

  delete test;

	return 0;
}