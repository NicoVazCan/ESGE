/**
 * \file ESGE_objActive.h
 * \brief Contains ESGE_ObjActive class for enabling and disabling game objects.
 */
#ifndef ESGE_OBJ_ACTIVE_H_
# define ESGE_OBJ_ACTIVE_H_


/**
 * Game object base abstract class for derived classes whose instances
 * can be enabled and disabled.
 */
class ESGE_ObjActive
{
  /**
   * Current state of the instance.
   */
  bool enabled = false;

public:
  /**
   * Constructor to be called from the derived classes.
   */
  ESGE_ObjActive(void);
  /**
   * Destructor to be called from the derived classes.
   */
  virtual ~ESGE_ObjActive(void) = 0;

  /**
   * This function must be called from the overriding function.
   * Callback called when the scene to which the instance belongs is
   * enabled.
   * 
   * \sa ESGE_ObjScene::OnStart
   * \sa ESGE_ObjActive::OnDisable
   */
  virtual void OnEnable(void);
  /**
   * This function must be called from the overriding function.
   * Callback called when the scene to which the instance belongs is
   * disabled.
   * 
   * \sa ESGE_ObjActive::OnEnable
   */
  virtual void OnDisable(void);

  /**
   * Use this function to know if the game object instance is
   * enabled.
   * 
   * \return true if is enabled, false otherwise.
   */
  bool IsEnabled(void) const;
};

#endif