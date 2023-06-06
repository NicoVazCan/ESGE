#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>


class ESGE_ObjSerial
{
  friend int ESGE_CmpObjSerial(
    const ESGE_ObjSerial *left,
    const ESGE_ObjSerial *right
  );
public:
  static ESGE_ObjSerial *list;
  ESGE_ObjSerial *next;
  const Uint16 id;
  
  ESGE_ObjSerial(Uint16 id);
  virtual ~ESGE_ObjSerial(void);
  virtual int Save(SDL_RWops *io) const = 0;
  virtual Uint16 GetTypeID(void) const = 0;
  static ESGE_ObjSerial *GetObj(Uint16 id);
};

typedef ESGE_ObjSerial *(*ESGE_Load)(SDL_RWops *io);

class ESGE_Loader
{
  static ESGE_Loader *list;
  ESGE_Loader *next;
  const Uint16 typeID;
  const ESGE_Load load;

  friend int ESGE_CmpLoader(
    const ESGE_Loader *left,
    const ESGE_Loader *right
  );
public:
  ESGE_Loader(Uint16 typeID, ESGE_Load load);
  ~ESGE_Loader(void);
  static ESGE_ObjSerial *Load(Uint16 typeID, SDL_RWops *io);
};

#endif