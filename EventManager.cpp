#include "EventManager.h"
#include "InputManager.h"
#include "SDL_events.h"
#include <queue>

using namespace GameLayer;

class EventPimpl
{
public:
  EventPimpl(){}
  ~EventPimpl(){}

  std::queue<SDL_Event> m_eventQueue;
};

EventManager::~EventManager(void)
{
  m_pimpl->m_eventQueue.empty();
}
EventManager::EventManager(void)
{
  m_pimpl.reset(new EventPimpl());
}

void EventManager::HandleEvent(void* a_event)
{
  m_pimpl->m_eventQueue.push(*static_cast<SDL_Event*>(a_event));
}

void EventManager::Update(float a_fDeltaTime)
{
  while (m_pimpl->m_eventQueue.size() > 0)
  {
    switch (m_pimpl->m_eventQueue.front().type)
	  {
	  case SDL_QUIT:
		  //exitapp = 1;
		  break;
	  case SDL_KEYDOWN:
      if (m_pimpl->m_eventQueue.front().key.keysym.sym == SDLK_ESCAPE)
		  {
			  //exitapp = 1;
			  // find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey
		  }
      Input->SetKeyPressed((Key)m_pimpl->m_eventQueue.front().key.keysym.sym);
		  break;
	  case SDL_KEYUP:
      Input->SetKeyUp((Key)m_pimpl->m_eventQueue.front().key.keysym.sym);
		  break;
	  case SDL_MOUSEMOTION:
      Input->SetMousePosition(m_pimpl->m_eventQueue.front().motion.x, m_pimpl->m_eventQueue.front().motion.y);
      Input->SetMouseMovement(m_pimpl->m_eventQueue.front().motion.xrel, m_pimpl->m_eventQueue.front().motion.yrel);
		  break;
	  case SDL_MOUSEBUTTONUP:
      Input->SetMouseButtonUp(m_pimpl->m_eventQueue.front().button.button);
		  break;
	  case SDL_MOUSEBUTTONDOWN:
      Input->SetMouseButtonPressed(m_pimpl->m_eventQueue.front().button.button);
		  break;
	  default:
		  // more info on events in SDL: http://sdl.beuc.net/sdl.wiki/SDL_Event
		  break;
	  }

    m_pimpl->m_eventQueue.pop();
  }
}