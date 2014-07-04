// Template, major revision 3
// IGAD/NHTV - Jacco Bikker - 2006-2009
#include "Core.h"

#include "string.h"
#include "surface.h"
#include "stdlib.h"
#include "template.h"
#include "game.h"

using namespace Tmpl8;

void Game::Init()
{
  GameLayerCore->Init(m_Screen);
}


void Game::Tick( float a_DT )
{
  GameLayerCore->Update(a_DT);
}


int Game::Update()
{
  return 0;
}

void Game::PassEvent(SDL_Event& a_event)
{
  GameLayerCore->PassEvent((void*)&a_event);
}