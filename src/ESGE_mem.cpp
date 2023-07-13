#include "ESGE_mem.h"
/*
void* operator new(size_t sz)
{
  void *ptr;

  if (sz == 0) ++sz;
  if (!(ptr = SDL_malloc(sz)))
  {
    SDL_OutOfMemory();
    return NULL;
  }
  return ptr;
}

void* operator new[](size_t sz)
{
  void *ptr;

  if (sz == 0) ++sz;
  if (!(ptr = SDL_malloc(sz)))
  {
    SDL_OutOfMemory();
    return NULL;
  }
  return ptr;
}
*/

void* operator new(SDL_UNUSED std::size_t sz, void* ptr)
{
  return ptr;
}

void* operator new[](SDL_UNUSED std::size_t sz, void* ptr)
{
  return ptr;
}


void operator delete(void* ptr)
{
  SDL_free(ptr);
}

void operator delete(void* ptr, SDL_UNUSED size_t size)
{
  SDL_free(ptr);
}
 
void operator delete[](void* ptr)
{
  SDL_free(ptr);
}
 
void operator delete[](void* ptr, SDL_UNUSED size_t size)
{
  SDL_free(ptr);
}


extern "C" void __cxa_pure_virtual()
{
  while(1);
}
