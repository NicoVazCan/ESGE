#ifndef ESGE_OBJ_UPDATE_H_
# define ESGE_OBJ_UPDATE_H_

class ESGE_ObjUpdate
{
  static ESGE_ObjUpdate *list;

  ESGE_ObjUpdate *next;
  bool enabledUpdate = false;

public:
  static void Update(void);

  ESGE_ObjUpdate(void);
  virtual ~ESGE_ObjUpdate(void) = 0;

  void EnableUpdate(void);
  void DisableUpdate(void);
  bool IsEnabledUpdate(void);

  virtual void OnUpdate(void);
};

#endif