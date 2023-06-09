#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>
# include "ESGE_objActive.h"

# define ESGE_CMP_OBJ_SERIAL(left, right) ((left)->id - (right)->id)

class ESGE_ObjSerial: public virtual ESGE_ObjActive
{
  friend int ESGE_CmpObjSerial(
    const ESGE_ObjSerial *left,
    const ESGE_ObjSerial *right
  );
public:
  const Uint16 id;

  ESGE_ObjSerial(Uint16 id);
  virtual ~ESGE_ObjSerial(void);
  virtual void OnSave(SDL_RWops *io) const;
  virtual Uint16 GetTypeID(void) const = 0;
};

typedef ESGE_ObjSerial *(*ESGE_Load)(SDL_RWops *io);

class ESGE_Loader
{
  static ESGE_Loader *list;
  ESGE_Loader *next;
  const Uint16 typeID;
  const ESGE_Load load;
  
public:
  ESGE_Loader(Uint16 typeID, ESGE_Load load);
  ~ESGE_Loader(void);
  static ESGE_ObjSerial *Load(Uint16 typeID, SDL_RWops *io);
};

#endif