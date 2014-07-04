#pragma once
#include "Singleton.h"

class EventPimpl;
namespace GameLayer{

  class EventManager : public Singleton<EventManager>
  {
    friend class Singleton<EventManager>;
  public:
	  ~EventManager(void);
    void HandleEvent(void* a_event);
    void Update(float a_fDeltaTime);
  private:
    EventManager(void);

    std::unique_ptr<EventPimpl> m_pimpl;
  };
};

#define Events EventManager::GetInstance()