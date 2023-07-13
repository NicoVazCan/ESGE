#ifndef ESGE_OBJ_EDITOR_H_
# define ESGE_OBJ_EDITOR_H_

# include <SDL2/SDL.h>
# include <ESGE_objCam.h>

class ESGE_ObjEditorCam: public ESGE_ObjCam
{
public:
  ESGE_ObjEditorCam(void);
  virtual ~ESGE_ObjEditorCam(void) override;
};

#endif