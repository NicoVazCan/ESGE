#ifndef ESGE_FILE_H_
# define ESGE_FILE_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"
# include "ESGE_mem.h"


class ESGE_File
{
public:
  unsigned watchers = 0;
  const char *fileName;
  const Uint64 fileID;
  ESGE_File *next;

  ESGE_File(const char *fileName);
  virtual ~ESGE_File(void);
};

template<class C>
class ESGE_FileMngr
{
  static ESGE_File *list;

  ESGE_FileMngr(void) {}
  ~ESGE_FileMngr(void) {}

public:
  static C *Watch(const char *fileName)
  {
    ESGE_File **node;
    Uint64 fileID;

    fileID = ESGE_Hash(fileName);

    for (
      node = &list;
      *node != NULL && (*node)->fileID < fileID;
      node = &(*node)->next
    );

    if (*node == NULL || (*node)->fileID != fileID)
    {
      ESGE_File *next = *node;

      *node = new C(fileName);
      (*node)->next = next;
    }

    ++((*node)->watchers);

    return (C*)*node;
  }
  static void Leave(C *item)
  {
    ESGE_File **node;

    for (
      node = &list;
      *node != NULL && (*node)->fileID < item->fileID;
      node = &(*node)->next
    );

    if (*node != NULL && (*node)->fileID == item->fileID)
    {
      --(*node)->watchers;

      if (!(*node)->watchers)
      {
        ESGE_File *next = ((ESGE_File*)item)->next;

        delete *node;

        *node = next;
      }
    }
  }
};

template<class C> ESGE_File *ESGE_FileMngr<C>::list = NULL;

#endif