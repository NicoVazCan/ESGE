#ifndef ESGE_OBJ_ACTIVE_H_
# define ESGE_OBJ_ACTIVE_H_


class ESGE_ObjActive
{
  bool enabled = false;

public:
  ESGE_ObjActive(void);
  virtual ~ESGE_ObjActive(void) = 0;

  virtual void OnEnable(void);
  virtual void OnDisable(void);

  bool IsEnabled(void) const;
  void SetEnabled(bool enabled);
};

#endif