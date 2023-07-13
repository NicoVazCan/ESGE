#include "ESGE_file.h"

ESGE_File::ESGE_File(const char *fileName)
{
  SDL_assert(fileName != NULL);
  
  SDL_strlcpy(this->fileName, fileName, ESGE_FILE_NAME_LEN);
  fileID = ESGE_Hash(fileName);
}

ESGE_File::~ESGE_File(void) {}
