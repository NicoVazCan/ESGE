#include "ESGE_file.h"

#include "ESGE_error.h"

ESGE_File::ESGE_File(const char *fileName):
  fileID(ESGE_Hash(fileName))
{
  SDL_assert(fileName != NULL);
  
  if (!(this->fileName = SDL_strdup(fileName)))
    ESGE_Error("Cannot allocate \"%s\" string", fileName);
}

ESGE_File::~ESGE_File(void)
{
  SDL_free((void*)fileName);
}
