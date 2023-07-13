#ifndef ESGE_MEM_H_
# define ESGE_MEM_H_

# include <SDL2/SDL.h>
/*
void* operator new(size_t sz);
void* operator new[](size_t sz);
*/
void* operator new(std::size_t sz, void* ptr);
void* operator new[](std::size_t sz, void* ptr);

void  operator delete(void* ptr);
void  operator delete(void* ptr, size_t size);
void  operator delete[](void* ptr);
void  operator delete[](void* ptr, size_t size);

#endif