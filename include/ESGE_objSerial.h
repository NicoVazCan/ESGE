/**
 * \file ESGE_objSerial.h
 * \brief Contains all the necessary classes for serialize game objects.
 */
#ifndef ESGE_OBJ_SERIAL_H_
# define ESGE_OBJ_SERIAL_H_

# include <SDL2/SDL.h>
# include "ESGE_hash.h"


/**
 * Use this macro in global context to set class \a CLASS as a game
 * object class to add its instances to game scenes ESGE_Scene using
 * ESGE_ObjScene::Create or the editor program tool.
 * 
 * \param CLASS the name of the game object class.
 * 
 * \sa ESGE_TYPE_FIELDS
 */
# define ESGE_TYPE(CLASS) \
static const ESGE_TypeImpl<CLASS> ESGE__type##CLASS( \
  ESGE_Hash(#CLASS), \
  NULL, \
  0 \
);

/**
 * Uses this macro to stop setting fields as storable in ESGE_FIELD
 * recursive calls.
 * 
 * \sa ESGE_FIELD
 * \sa ESGE_TYPE_FIELDS
 */
# define ESGE_END_FIELD

/**
 * Uses this macro recursively to set as storable field the member
 * variable with \a NAME of the game object class passed to
 * ESGE_TYPE_FIELDS.
 * To stop setting fields, use macro ESGE_END_FIELD as \a NEXT.
 * 
 * \param NAME the name of the member variable.
 * \param GET the static member function to get the value of the
 *            member variable \a NAME.
 *            The signature of this function should be as:
 *            "T get(void *obj)", being "T" a numerical type, "get"
 *            its name, and "obj" a pointer to the instance, or as:
 *            "char* get(void *obj, char *str, size_t n)" for char
 *            strings.
 * \param SET the static member function to set the value of the
 *            member variable \a NAME.
 *            The signature of this function should be as:
 *            "void set(void *obj, T value)", being "T" a numerical
 *            type, "set" its name, "obj" a pointer to the instance,
 *            and "value" the new value for the member variable \a
 *            NAME, or as:
 *            "void set(void *obj, const char *str)" for char strings.
 * \param NEXT another ESGE_FIELD macro call or ESGE_END_FIELD.
 * 
 * \sa ESGE_END_FIELD
 * \sa ESGE_TYPE_FIELDS
 */
# define ESGE_FIELD(NAME, GET, SET, NEXT) \
ESGE_Field(#NAME, GET, SET), NEXT

/**
 * Use this macro in global context to set class \a CLASS as a game
 * object class to add its instances to game scenes ESGE_Scene using
 * ESGE_ObjScene::Create or the editor program tool when they have
 * fields that need to be saved.
 * 
 * \param CLASS the name of the game object class.
 * \param FIELDS the recursive result of the macro ESGE_FIELD.
 * 
 * \sa ESGE_TYPE
 */
# define ESGE_TYPE_FIELDS(CLASS, FIELDS) \
static const ESGE_Field ESGE__fields##CLASS[] = \
{ FIELDS }; \
static const ESGE_TypeImpl<CLASS> ESGE__type##CLASS( \
  ESGE_Hash(#CLASS), \
  ESGE__fields##CLASS, \
  SDL_arraysize(ESGE__fields##CLASS) \
);


/**
 * The max size of a serializable string.
 */
# define ESGE_MAX_STR 256

/**
 * Internal use only.
 * A template struct to store the functions for get and set the value
 * of a member variable of a serializable game object.
 * 
 * \tparam T a numerical type.
 */
template<typename T>
struct ESGE_FieldValue
{
  /**
   * Value function getter.
   */
  T    (*const get)(void *obj);
  /**
   * Value function setter.
   */
  void (*const set)(void *obj, T value);
};
/**
 * Internal use only.
 * A template struct definition to store the functions for get and set
 * the value of a char string member variable of a serializable game
 * object.
 */
template<>
struct ESGE_FieldValue<char*>
{
  /**
   * Value function getter.
   */
  char* (*const get)(void *obj, char* str, size_t n);
  /**
   * Value function setter.
   */
  void  (*const set)(void *obj, const char* str);
};

/**
 * Internal use only.
 * A struct that represent a serializable game object field or
 * member variable.
 */
struct ESGE_Field
{
  /**
   * Internal use only.
   * The field type.
   */
  const enum {C,I,L,LL,F,S} type;
  /**
   * Internal use only.
   * The field identifier.
   */
  const Uint64 id;
  /**
   * Internal use only.
   * The name of the field.
   */
  const char *name;
  /**
   * Internal use only.
   * A union that stores the functions to get and set the field value.
   */
  union
  {
    ESGE_FieldValue<char>                   valueC;
    ESGE_FieldValue<int>                    valueI;
    ESGE_FieldValue<long>                   valueL;
    ESGE_FieldValue<long long>              valueLL;
    ESGE_FieldValue<float>                  valueF;
    ESGE_FieldValue<char*>                  valueS;
  };

  /**
   * Internal use only.
   * The overloaded construct for a field of type char.
   */
  ESGE_Field(
    const char* name,
    char (*get)(void *obj),
    void (*set)(void *obj, char value)
  );
  /**
   * Internal use only.
   * The overloaded construct for a field of type int.
   */
  ESGE_Field(
    const char* name,
    int (*get)(void *obj),
    void (*set)(void *obj, int value)
  );
  /**
   * Internal use only.
   * The overloaded construct for a field of type long.
   */
  ESGE_Field(
    const char *name,
    long (*get)(void *obj),
    void (*set)(void *obj, long value)
  );
  /**
   * Internal use only.
   * The overloaded construct for a field of type long long.
   */
  ESGE_Field(
    const char *name,
    long long (*get)(void *obj),
    void (*set)(void *obj, long long value)
  );
  /**
   * Internal use only.
   * The overloaded construct for a field of type float.
   */
  ESGE_Field(
    const char *name,
    float (*get)(void *obj),
    void (*set)(void *obj, float value)
  );
  /**
   * Internal use only.
   * The overloaded construct for a field of type char*.
   */
  ESGE_Field(
    const char *name,
    char* (*get)(void *obj, char *str, size_t n),
    void (*set)(void *obj, const char *str)
  );

  /**
   * Internal use only.
   * This function gives the field value as a char string.
   * 
   * \param obj the game object instance.
   * \param value the buffer to write the value as a char string.
   * \param len the max char to write in the \a value buffer.
   */
  void GetValue(void *obj, char *value, size_t len) const;
  /**
   * Internal use only.
   * This function reads the field value.
   * 
   * \param obj the game object instance.
   * \param value the buffer to write the value as it is.
   * \param len the max char to write in the \a value buffer in case
   *        of char string field.
   */
  void GetValue(void *obj, void *value, size_t len) const;

  /**
   * Internal use only.
   * This function writes the field value from a char string.
   * 
   * \param obj the game object instance.
   * \param value the char string with the new value.
   */
  void SetValue(void *obj, const char *value) const;
  /**
   * Internal use only.
   * This function writes the field value from a pointer.
   * 
   * \param obj the game object instance.
   * \param value the pointer to a variable with the value.
   */
  void SetValue(void *obj, const void *value) const;
};


/**
 * Internal use only.
 * This class instances represent a serializable game object class to
 * create new instances and get/set their field values dynamically. 
 */
class ESGE_Type
{
  /**
   * Internal use only.
   * The global sorted list of all game object classes.
   */
  static ESGE_Type *list;
  /**
   * Internal use only.
   * The next game object class.
   */
  ESGE_Type *next;

public:
  /**
   * Internal use only.
   * This function gets the game object class.
   * 
   * \param id the game object class id to find.
   * \return the game object class or null if not found. Check
   *         SDL_GetError for get more info about error.
   */
  static const ESGE_Type *Get(Uint64 id);
  /**
   * Internal use only.
   * This function gets the game object class.
   * 
   * \param typeName the game object class name to find.
   * \return the game object class or null if not found. Check
   *         SDL_GetError for get more info about error.
   */
  static const ESGE_Type *Get(const char *typeName);

  /**
   * Internal use only.
   * The identifier of the game object class.
   */
  const Uint64 id;
  /**
   * Internal use only.
   * The serializable fields of the game object class.
   */
  const ESGE_Field *const fields;
  /**
   * Internal use only.
   * The serializable field number of the game object class.
   */
  const size_t nFields;

  /**
   * Internal use only.
   * Construct for create ESGE_Type instances that represent game
   * object class.
   * This must be called during program start before main, this is
   * achieved by defining a of global variable for each game object
   * class.
   * 
   * \param id its identifier.
   * \param fields its fields.
   * \param nFields the number of its fields.
   */
  ESGE_Type(Uint64 id, const ESGE_Field *fields, size_t nFields);
  /**
   * Internal use only.
   * Destructor for eliminate ESGE_Type instances that represent game
   * object class.
   * This must be called during program end after main exit, this is
   * achieved by defining a of global variable for each game object
   * class.
   */
  virtual ~ESGE_Type(void);

  /**
   * Internal use only.
   * Function to find a serializable field from a game object class.
   * 
   * \param fieldId the field id.
   * \return the field or NULL if does not exist. Check
   *         SDL_GetError for get more info about error.
   */
  const ESGE_Field *GetField(Uint64 fieldId) const;
  /**
   * Internal use only.
   * Function to find a serializable field from a game object class.
   * 
   * \param fieldName the field name.
   * \return the field or NULL if does not exist. Check
   *         SDL_GetError for get more info about error.
   */
  const ESGE_Field *GetField(const char *fieldName) const;

  /**
   * Internal use only.
   * Pure virtual function to create a new instance of the game
   * object class.
   * This must be defined from derived classes with the concrete
   * game object class.
   * 
   * \return the new game object class instance.
   */
  virtual void *New(void) const = 0;
};

/**
 * A template class for defining new ESGE_Type derived classes.
 * This classes are a singlenton where its only instance represent the
 * concrete game object class of \a C.
 * 
 * \tparam C the concrete game object class.
 */
template<class C>
class ESGE_TypeImpl: ESGE_Type
{
  /**
   * Internal use only.
   * Pure virtual member function override for create instances of
   * the \a C concrete game object class.
   * 
   * \return the new game concrete object class instance.
   */
  virtual void *New(void) const
  {
    return new C;
  }

public:
  /**
   * Internal use only.
   * Construct for create ESGE_TypeImpl instances that represent
   * concrete game object class.
   * This must be called during program start before main, this is
   * achieved by defining a of global variable for each game object
   * class.
   * 
   * \param id its identifier.
   * \param fields its fields.
   * \param nFields the number of its fields.
   */
  ESGE_TypeImpl(Uint64 id, const ESGE_Field *fields, size_t nFields):
    ESGE_Type(id, fields, nFields)
  {}
  /**
   * Internal use only.
   * Destructor for eliminate ESGE_TypeImpl instances that represent
   * concrete game object class.
   * This must be called during program end after main exit, this is
   * achieved by defining a of global variable for each game object
   * class.
   */
  virtual ~ESGE_TypeImpl(void)
  {}
};

/**
 * Game object base abstract class for derived classes whose instances
 * can be serialized.
 */
class ESGE_ObjSerial
{
public:
  /**
   * Read only.
   * ESGE_Type identifier of the instance concrete game object class.
   */
  Uint64 typeID = 0;

  /**
   * Constructor to be called from the derived classes.
   */
  ESGE_ObjSerial(void);
  /**
   * Destructor to be called from the derived classes.
   */
  virtual ~ESGE_ObjSerial(void) = 0;

  /**
   * Internal use only.
   * Reads all instance fields stored in the scene file.
   * 
   * \param io the file stream to read the fields from.
   */
  void Load(SDL_RWops *io);
  /**
   * Internal use only.
   * Writes all instance fields stored in the scene file.
   * 
   * \param io the file stream to write the fields to.
   */
  void Save(SDL_RWops *io);
};

#endif