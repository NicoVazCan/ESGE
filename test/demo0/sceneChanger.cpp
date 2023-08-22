#include "ESGE_scene.h"

#include "ESGE_objEvent.h"
#include "ESGE_objSerial.h"

#define SCENE_FILE_LEN 128

class ObjSceneChanger: public ESGE_ObjScene, public ESGE_ObjKeyEvent
{
  char sceneFile[SCENE_FILE_LEN] = "scene.bin";
  SDL_Keycode key = 0;

public:
  static char* GetSceneFile(void *obj, char *str, size_t n)
  {
    SDL_strlcpy(str, ((ObjSceneChanger*)obj)->sceneFile, n);
    return str;
  }
  static void SetSceneFile(void *obj, const char *str)
  {
    SDL_strlcpy(
      ((ObjSceneChanger*)obj)->sceneFile,
      str,
      SCENE_FILE_LEN
    );
  }

  static int GetKey(void *obj)
  {
    return ((ObjSceneChanger*)obj)->key;
  }
  static void SetKey(void *obj, int value)
  {
    ((ObjSceneChanger*)obj)->key = value;
  }


  ObjSceneChanger(void)
  {}
  virtual ~ObjSceneChanger(void) override
  {}


  virtual void OnEnable(void) override
  {
    ESGE_ObjScene::OnEnable();
    EnableKeyEvent();
  }
  virtual void OnDisable(void) override
  {
    ESGE_ObjScene::OnDisable();
    DisableKeyEvent();
  }
  
  virtual void OnKeyDown(
    SDL_Keycode key,
    SDL_UNUSED SDL_Keymod mod
  ) override
  {
    if (key == this->key)
      ESGE_SceneMngr::ChangeScene(sceneFile);
  }
};


ESGE_TYPE_FIELDS(
  ObjSceneChanger,
  ESGE_FIELD(
    S,
    sceneFile,
    ObjSceneChanger::GetSceneFile,
    ObjSceneChanger::SetSceneFile,
    ESGE_FIELD(
      I,
      key,
      ObjSceneChanger::GetKey,
      ObjSceneChanger::SetKey,
      ESGE_END_FIELD
    )
  )
)
