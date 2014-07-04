#include "Core.h"
#include "EventManager.h"
#include "InputManager.h"
#include "RootNode.h"
#include "Texture.h"
#include "TriangulatedObject.h"
#include <time.h>
#include <iostream>

using namespace GameLayer;

std::shared_ptr<TriangulatedObject> triangulatedObject;
int Core::ScreenWidth, Core::ScreenHeight;

Core::Core()
{
}

void Core::Init(void* a_pGraphics)
{
  srand(time(0));
  m_rootNode.reset(new RootNode());
  Texture::SetMissing("assets/missing.png");

  TimerRDTSC timer;
  timer.Start();
  triangulatedObject = std::shared_ptr<TriangulatedObject>(
    new TriangulatedObject(m_rootNode, "triObj", 1024)
  );
  timer.Stop();
  std::cout << timer.Interval() << std::endl;
  m_rootNode->AddChild(triangulatedObject);
  m_rootNode->GetTransform()->Scale(2);
  m_rootNode->GetTransform()->Translate(Vector2f(-ScreenWidth / 2, -ScreenHeight / 2));
}

bool Core::Update(float a_fDeltaTime)
{
  m_rootNode->Process(a_fDeltaTime);
  m_rootNode->Render();

  if (Input->IsKeyDown(KEY_LEFT))
  {
    m_rootNode->GetTransform()->Translate(Vector2f(a_fDeltaTime * 240, 0));
  }
  if (Input->IsKeyDown(KEY_RIGHT))
  {
    m_rootNode->GetTransform()->Translate(Vector2f(a_fDeltaTime * -240, 0));
  }
  if (Input->IsKeyDown(KEY_UP))
  {
    m_rootNode->GetTransform()->Translate(Vector2f(0, a_fDeltaTime * -240));
  }
  if (Input->IsKeyDown(KEY_DOWN))
  {
    m_rootNode->GetTransform()->Translate(Vector2f(0, a_fDeltaTime * 240));
  }
  if (Input->IsKeyDown(KEY_m))
  {
    m_rootNode->GetTransform()->Scale(1 + a_fDeltaTime);
  }
  if (Input->IsKeyDown(KEY_n))
  {
    m_rootNode->GetTransform()->Scale(1 - a_fDeltaTime);
  }

  HandleActions();

  Input->Update(a_fDeltaTime);
  Events->Update(a_fDeltaTime);

  return false;
}

void Core::PassEvent(void* a_event)
{
  Events->HandleEvent(a_event);
}

std::weak_ptr<Object> Core::FindObject(unsigned int a_id)
{
  return m_rootNode->FindObject(a_id);
}

void Core::FindObjectsByName
(std::string a_name, std::vector<std::weak_ptr<Object>>& a_objectVec)
{
  m_rootNode->FindObjectsByName(a_name, a_objectVec);
}


void Core::HandleActions()
{
  
}