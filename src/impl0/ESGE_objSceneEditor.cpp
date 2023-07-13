#include "ESGE_objSceneEditor.h"
#define SGLIB_ASSERT SDL_assert
#include "../sglib.h"

#define CMD_LEN 32
#define ESGE_CMP_TEXT_LOADER(left, right) (  \
  (left)->typeID - (right)->typeID      \
)

ESGE_ObjSceneEditor::ESGE_ObjSceneEditor(const char *file)
{
  SDL_strlcpy(this->file, file, ESGE_OBJ_SCENE_FILE_LEN-1);
}

ESGE_ObjSceneEditor::~ESGE_ObjSceneEditor(void) {}

void
ESGE_ObjSceneEditor::OnQuit(void)
{
  scene->Disable();
  OnDisable();
}

void
ESGE_ObjSceneEditor::RunCMD(void)
{
  scene = new ESGE_ObjScene(file);

  for(;;)
  {
    const char* delim = " \n\r\t\b", *help =
      "Commands:\n"
      "  add [TYPE_ID]          calls the TYPE_ID!=0 object loader to "
      "create a new instance and add it to the scene.\n"
      "  delete [TYPE_ID] [ID]  deletes the instance with TYPE_ID!=0 "
      "and ID!=0 from the scene.\n"
      "  play                   saves and enables scene.\n"
      "  exit                   saves and close scene editor\n";
    char cmd[CMD_LEN], *tkn, *saveptr;


    fputs("> ", stdout);
    fgets(cmd, CMD_LEN, stdin);
    tkn = SDL_strtokr(cmd, delim, &saveptr);

    if (!SDL_strcmp(tkn, "add"))
    {
      Uint16 typeID;
      ESGE_ObjSerial *obj;

      tkn = SDL_strtokr(NULL, delim, &saveptr);

      if (tkn == NULL || !(typeID = SDL_atoi(tkn)))
      {
        puts(help);
        continue;
      }

      obj = ESGE_TextLoader::Load(typeID);

      if (obj != NULL)
      {
        ESGE_ObjInScene *inObj = dynamic_cast<ESGE_ObjInScene*>(obj);
        SDL_assert(inObj != NULL);
        scene->AddObj(inObj);
      }
    }
    else if (!SDL_strcmp(tkn, "delete"))
    {
      Uint16 typeID, id;
      ESGE_ObjInScene *obj;

      tkn = SDL_strtokr(NULL, delim, &saveptr);

      if (tkn == NULL || !(typeID = SDL_atoi(tkn)))
      {
        puts(help);
        continue;
      }

      tkn = SDL_strtokr(NULL, delim, &saveptr);

      if (tkn == NULL || !(id = SDL_atoi(tkn)))
      {
        puts(help);
        continue;
      }

      for (
        obj = scene->objList;
        obj != NULL && obj->GetTypeID() < typeID;
        obj = obj->next
      );

      if (obj != NULL && obj->GetTypeID() == typeID)
      {
        for (; obj != NULL && obj->id < id; obj = obj->next);

        if (obj != NULL && obj->id == id)
        {
          scene->DelObj(obj);
          delete obj;

          continue;
        }
      }

      printf(
        "ObjInScene with typeID=%d and id=%d not found\n",
        typeID,
        id
      );
    }
    else if (!SDL_strcmp(tkn, "play"))
    {
      OnEnable();

      scene->Save();
      scene->Enable();
      ESGE_MainLoop();
      delete scene;
      break;
    }
    else if (!SDL_strcmp(tkn, "exit"))
    {
      scene->Save();
      delete scene;
      break;
    }
    else
    {
      puts(help);
    }
  }
}


ESGE_TextLoader *ESGE_TextLoader::list = NULL;

ESGE_TextLoader::ESGE_TextLoader(
  Uint16 typeID,
  ESGE_TextLoad load
): typeID(typeID), load(load)
{
  SGLIB_SORTED_LIST_ADD(
    ESGE_TextLoader,
    list,
    this,
    ESGE_CMP_TEXT_LOADER,
    next
  );
}

ESGE_TextLoader::~ESGE_TextLoader(void)
{
  SGLIB_SORTED_LIST_DELETE(ESGE_TextLoader, list, this, next);
}

ESGE_ObjSerial*
ESGE_TextLoader::Load(Uint16 typeID)
{
  ESGE_TextLoader *p;

  for (p = list; p != NULL && p->typeID < typeID; p = p->next);

  if (p != NULL && p->typeID == typeID) return p->load();
  return NULL;
}
