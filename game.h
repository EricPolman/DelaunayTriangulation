// Template, major revision 3, beta
// IGAD/NHTV - Jacco Bikker - 2006-2009

#pragma once
#include "surface.h"
#include "SDL_events.h"

namespace Tmpl8 {

  class Game
  { 
  public:
	  void SetTarget( Surface* a_Surface ) { m_Screen = a_Surface; }
	  void Init();
	  void Tick( float a_DT );
    void PassEvent(SDL_Event& a_event);
	  int  Update();

  private:
	  Surface* m_Screen;
  };
}; // namespace Tmpl8