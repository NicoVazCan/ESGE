#ifndef ESGE_OBJ_PHYSIC_H_
# define ESGE_OBJ_PHYSIC_H_


class ESGE_ObjPhysic
{
  static ESGE_ObjPhysic *list;

  ESGE_ObjPhysic *next;
  bool enabledPhysic = false;

public:
  static void Physic(void);

  ESGE_ObjPhysic(void);
  virtual ~ESGE_ObjPhysic(void) = 0;

  void EnablePhysic(void);
  void DisablePhysic(void);
  bool IsEnabledPhysic(void);

  virtual void OnPhysic(void);
};

#endif