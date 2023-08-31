/**
 * \file ESGE_file.h
 * \brief Contains the definitions of file class and fileMngr class template for file managing.
 */

#ifndef ESGE_FILE_H_
# define ESGE_FILE_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"


/**
 * \class ESGE_File
 * \brief Represents a file and manages watchers using the watcher pattern to ensure just a instance per file.
 */
class ESGE_File
{
public:
  unsigned watchers = 0; /**< Number of watchers for the file. */
  const char *fileName; /**< Path to the file. */
  const Uint64 fileID; /**< Hashed ID of the file. */
  ESGE_File *next; /**< Pointer to the next ESGE_File object in the list. */

  /**
   * \brief Constructor.
   *
   * Initializes the file with the provided file name.
   *
   * \param fileName Path to the file.
   */
  ESGE_File(const char *fileName);
  /**
   * \brief Destructor.
   *
   * Cleans up resources associated with the file.
   */
  virtual ~ESGE_File(void);
};

/**
 * \class ESGE_FileMngr
 * \brief Manages file watching and caching with singleton pattern.
 */
template<class C>
class ESGE_FileMngr
{
  static ESGE_File *list; /**< Pointer to the list of watched files. */

  /**
   * \brief Private constructor (singleton).
   */
  ESGE_FileMngr(void) {}
  /**
   * \brief Private destructor (singleton).
   */
  ~ESGE_FileMngr(void) {}

public:
  /**
   * \brief Watch a file and return its instance.
   *
   * Creates or retrieves an instance of the watched file and increments its
   * watcher count.
   *
   * \param fileName Path to the file.
   * \return A pointer to the watched file instance.
   */
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
  /**
   * \brief Stop watching a file and clean up if needed.
   *
   * Decrements the watcher count for the file. If there are no more watchers,
   * the file instance is cleaned up.
   *
   * \param item Pointer to the watched file instance.
   */
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