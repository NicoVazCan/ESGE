/**
 * \file ESGE_objEvent.h
 * \brief Contains the definitions of classes related to handling events.
 */
#ifndef ESGE_OBJ_EVENT_H_
# define ESGE_OBJ_EVENT_H_

# include <SDL2/SDL.h>


/**
 * \class ESGE_Event
 * \brief Represents objects with event handling behavior.
 */
class ESGE_Event
{
  static ESGE_Event *list; /**< Pointer to the first game object with event handling behavior in the list. */

  ESGE_Event *next = NULL; /**< Pointer to the next game object with event handling behavior in the list. */
  bool enabledEvent = false; /**< Flag indicating if event handling is enabled for this game object. */

public:
  /**
   * \brief Static method to handle events for all enabled objects in the list.
   * Internal use only.
   * Called at the beggining of the main loop.
   */
  static void Event(void);

  /**
   * \brief Default constructor for ESGE_Event.
   */
  ESGE_Event(void);
  /**
   * \brief Virtual destructor for ESGE_Event.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_Event(void) = 0;

  /**
   * \brief Enable event handling for the game object.
   */
  void EnableEvent(void);
  /**
   * \brief Disable event handling for the game object.
   */
  void DisableEvent(void);
  /**
   * \brief Check if event handling is enabled for the game object.
   * \return True if event handling is enabled, false otherwise.
   */
  bool IsEnabledEvent(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called for each event of the event loop.
   */
  virtual void OnEvent(void);
};

/**
 * \class ESGE_ObjKeyEvent
 * \brief Represents objects with key event handling behavior.
 */
class ESGE_ObjKeyEvent
{
  static ESGE_ObjKeyEvent *list; /**< Pointer to the first game object with key event handling behavior in the list. */

  ESGE_ObjKeyEvent *next = NULL; /**< Pointer to the next game object with key event handling behavior in the list. */
  bool enabledKeyEvent = false; /**< Flag indicating if key event handling is enabled for this game object. */

public:
  /**
   * \brief Static method to handle key down events for all enabled objects in the list.
   */
  static void KeyDown(void);
  /**
   * \brief Static method to handle key up events for all enabled objects in the list.
   */
  static void KeyUp(void);

  /**
   * \brief Default constructor for ESGE_ObjKeyEvent.
   */
  ESGE_ObjKeyEvent(void);
  /**
   * \brief Virtual destructor for ESGE_ObjKeyEvent.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjKeyEvent(void) = 0;

  /**
   * \brief Enable key event handling for the game object.
   */
  void EnableKeyEvent(void);
  /**
   * \brief Disable key event handling for the game object.
   */
  void DisableKeyEvent(void);
  /**
   * \brief Check if key event handling is enabled for the game object.
   * \return True if key event handling is enabled, false otherwise.
   */
  bool IsEnabledKeyEvent(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a key down event is triggered for the game object.
   * 
   * \param key The key code of the pressed key.
   * \param mod The key modifiers (e.g., Shift, Ctrl) associated with the key press.
   * 
   * \sa SDL_KeyCode
   * \sa SDL_Keymod
   */
  virtual void OnKeyDown(SDL_Keycode key, SDL_Keymod mod);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a key up event is triggered for the game object.
   * 
   * \param key The key code of the released key.
   * \param mod The key modifiers (e.g., Shift, Ctrl) associated with the key release.
   * 
   * \sa SDL_KeyCode
   * \sa SDL_Keymod
   */
  virtual void OnKeyUp(SDL_Keycode key, SDL_Keymod mod);
};

/**
 * \class ESGE_ObjMouseEvent
 * \brief Represents objects with mouse event handling behavior.
 */
class ESGE_ObjMouseEvent
{
  static ESGE_ObjMouseEvent *list; /**< Pointer to the first game object with mouse event handling behavior in the list. */

  ESGE_ObjMouseEvent *next = NULL; /**< Pointer to the next game object with mouse event handling behavior in the list. */
  bool enabledMouseEvent = false; /**< Flag indicating if mouse event handling is enabled for this game object. */

public:
  /**
   * \brief Static method to handle mouse move events for all enabled objects in the list.
   */
  static void MouseMove(void);
  /**
   * \brief Static method to handle mouse button down events for all enabled objects in the list.
   */
  static void MouseButtonDown(void);
  /**
   * \brief Static method to handle mouse button up events for all enabled objects in the list.
   */
  static void MouseButtonUp(void);
  /**
   * \brief Static method to handle mouse wheel events for all enabled objects in the list.
   */
  static void MouseWheel(void);

  /**
   * \brief Default constructor for ESGE_ObjMouseEvent.
   */
  ESGE_ObjMouseEvent(void);

  /**
   * \brief Virtual destructor for ESGE_ObjMouseEvent.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjMouseEvent(void) = 0;

  /**
   * \brief Enable mouse event handling for the game object.
   */
  void EnableMouseEvent(void);
  /**
   * \brief Disable mouse event handling for the game object.
   */
  void DisableMouseEvent(void);
  /**
   * \brief Check if mouse event handling is enabled for the game object.
   * \return True if mouse event handling is enabled, false otherwise.
   */
  bool IsEnabledMouseEvent(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a mouse move event is triggered for the game object.
   * \param x The x-coordinate of the mouse cursor.
   * \param y The y-coordinate of the mouse cursor.
   * \param relX The relative x movement of the mouse cursor.
   * \param relY The relative y movement of the mouse cursor.
   */
  virtual void OnMouseMove(int x, int y, int relX, int relY);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a mouse button down event is triggered for the game object.
   * \param x The x-coordinate of the mouse cursor.
   * \param y The y-coordinate of the mouse cursor.
   * \param button The button that was pressed (e.g., SDL_BUTTON_LEFT).
   */
  virtual void OnMouseButtonDown(int x, int y, Uint8 button);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a mouse button up event is triggered for the game object.
   * \param x The x-coordinate of the mouse cursor.
   * \param y The y-coordinate of the mouse cursor.
   * \param button The button that was released (e.g., SDL_BUTTON_LEFT).
   */
  virtual void OnMouseButtonUp(int x, int y, Uint8 button);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a mouse wheel event is triggered for the game object.
   * \param x The x-coordinate of the mouse cursor.
   * \param y The y-coordinate of the mouse cursor.
   * \param wheelX The amount of horizontal mouse wheel movement.
   * \param wheelY The amount of vertical mouse wheel movement.
   */
  virtual void OnMouseWheel(int x, int y, int wheelX, int wheelY);
};


/**
 * \class ESGE_ObjJoyEvent
 * \brief Represents objects with joystick event handling behavior.
 */
class ESGE_ObjJoyEvent
{
  static ESGE_ObjJoyEvent *list; /**< Pointer to the first game object with joystick event handling behavior in the list. */

  ESGE_ObjJoyEvent *next = NULL; /**< Pointer to the next game object with joystick event handling behavior in the list. */
  bool enabledJoyEvent = false; /**< Flag indicating if joystick event handling is enabled for this game object. */

public:
  /**
   * \brief Static method to handle joystick axis events for all enabled objects in the list.
   */
  static void JoyAxis(void);
  /**
   * \brief Static method to handle joystick ball events for all enabled objects in the list.
   */
  static void JoyBall(void);
  /**
   * \brief Static method to handle joystick hat events for all enabled objects in the list.
   */
  static void JoyHat(void);
  /**
   * \brief Static method to handle joystick button down events for all enabled objects in the list.
   */
  static void JoyButtonDown(void);
  /**
   * \brief Static method to handle joystick button up events for all enabled objects in the list.
   */
  static void JoyButtonUp(void);
  /**
   * \brief Static method to handle joystick device add events for all enabled objects in the list.
   */
  static void JoyDeviceAdd(void);
  /**
   * \brief Static method to handle joystick device remove events for all enabled objects in the list.
   */
  static void JoyDeviceRem(void);
  /**
   * \brief Static method to handle joystick battery events for all enabled objects in the list.
   */
  static void JoyBattery(void);

  SDL_JoystickID joyID = -1; /**< The ID of the joystick which the game object will hear for. -1 by default means will be hear for any of the joysticks.*/

  /**
   * \brief Default constructor for ESGE_ObjJoyEvent.
   */
  ESGE_ObjJoyEvent(void);
  /**
   * \brief Virtual destructor for ESGE_ObjJoyEvent.
   * This class is meant to be used as a base class, so the destructor is declared as virtual.
   */
  virtual ~ESGE_ObjJoyEvent(void) = 0;

  /**
   * \brief Enable joystick event handling for the game object.
   */
  void EnableJoyEvent(void);
  /**
   * \brief Disable joystick event handling for the game object.
   */
  void DisableJoyEvent(void);
  /**
   * \brief Check if joystick event handling is enabled for the game object.
   * \return True if joystick event handling is enabled, false otherwise.
   */
  bool IsEnabledJoyEvent(void);

  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick axis event is triggered for the game object.
   * \param axis The axis number.
   * \param value The axis value (range: -32768 to 32767).
   */
  virtual void OnJoyAxis(Uint8 axis, Sint16 value);
    /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick ball event is triggered for the game object.
   * \param ball The ball number.
   * \param relX The relative x movement of the ball.
   * \param relY The relative y movement of the ball.
   */
  virtual void OnJoyBall(Uint8 ball, Sint16 relX, Sint16 relY);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick hat event is triggered for the game object.
   * \param hat The hat number.
   * \param value The hat value.
   */
  virtual void OnJoyHat(Uint8 hat, Uint8 value);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick button down event is triggered for the game object.
   * \param button The button number.
   * 
   * \sa SDL hat positions
   */
  virtual void OnJoyButtonDown(Uint8 button);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick button up event is triggered for the game object.
   * \param button The button number.
   */
  virtual void OnJoyButtonUp(Uint8 button);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick device add event is triggered for the game object.
   * \param joyID The ID of the added joystick.
   */
  virtual void OnJoyDeviceAdd(SDL_JoystickID joyID);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick device remove event is triggered for the game object.
   * \param joyID The ID of the removed joystick.
   * 
   * \sa SDL_JoystickID
   */
  virtual void OnJoyDeviceRem(SDL_JoystickID joyID);
  /**
   * \brief Virtual function to be overridden by derived classes.
   * This function is called when a joystick battery event is triggered for the game object.
   * \param level The battery power level.
   * 
   * \sa SDL_JoystickID
   * 
   * \sa SDL_JoystickPowerLevel
   */
  virtual void OnJoyBattery(SDL_JoystickPowerLevel level);
};


/**
 * \brief SDL event variable to store incoming events.
 * \details This variable is used to store SDL events for processing within the event loop.
 */
extern SDL_Event ESGE_event;
/**
 * \brief Flag to indicate whether the program should exit the event loop.
 * \details This boolean flag is used to determine whether the main loop should continue running or exit. Set as true to finish game execution.
 */
extern bool ESGE_quit;

/**
 * \brief Handle SDL events within a loop.
 * \details This function is responsible for processing SDL events and dispatching them to relevant event handlers.
 */
void ESGE_EventLoop(void);

#endif
