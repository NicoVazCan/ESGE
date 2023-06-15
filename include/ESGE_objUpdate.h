#ifndef ESGE_OBJ_UPDATE_H_
# define ESGE_OBJ_UPDATE_H_

# include "ESGE_plc.h"
# include "ESGE_event.h"
# include "ESGE_objActive.h"

#define ESGE_OBJ_UPDATE_PRI ESGE_EVENT_LOOP_PRI+1

class ESGE_ObjUpdate: public virtual ESGE_ObjActive
{
  friend void ESGE_OnUpdate(void *userdata);

public:
  ESGE_ObjUpdate(void);
  virtual ~ESGE_ObjUpdate(void);
  virtual void OnEnable(void) override;
  virtual void OnDisable(void) override;

protected:
  virtual void OnUpdate(void) = 0;
};

#endif