#pragma once
#include "Keys.h"
#include "Vector2.h"
#include "Singleton.h"
#include <map>

namespace GameLayer
{
  

  class InputManager : public Singleton<InputManager>
  {
  public:
	  ~InputManager(void);
	  InputManager(void);
	
	  bool IsKeyPressed(Key a_key);
    bool IsKeyDown(Key a_key);
    bool IsKeyUp(Key a_key);
	
    void SetKeyPressed(Key a_key);
	  void SetKeyUp(Key a_key);

	  bool IsMouseButtonPressed(Button a_mouseButton);
    bool IsMouseButtonDown(Button a_mouseButton);
    bool IsMouseButtonUp(Button a_mouseButton);
	
    void SetMouseButtonPressed(Button a_mouseButton);
    void SetMouseButtonUp(Button a_mouseButton);
	
	  void SetMousePosition(int x, int y);
	  void SetMouseMovement(int x, int y);
	  Vector2i GetMousePosition();
  
    virtual void Update(float a_fDeltaTime);
  private:
	
	  Vector2i m_mousePosition;
	  Vector2i m_mouseMovement;
	
    std::map<Key, bool> m_keysDown;
    std::map<Key, bool> m_keysPressed;
    std::map<Key, bool> m_keysUp;
	
    std::map<Button, bool> m_mouseButtonDown;
    std::map<Button, bool> m_mouseButtonPressed;
    std::map<Button, bool> m_mouseButtonUp;
  };
};
#define Input InputManager::GetInstance()
