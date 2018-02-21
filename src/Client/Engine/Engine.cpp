#include "Engine.h"

#include "../Input/ActionManager.h"

CEngine::CEngine()
{

}

CEngine::~CEngine()
{

}

void CEngine::ProcessInputs()
{
	m_ActionManager->Update();
}

void CEngine::Update(float aDeltaTime)
{

}

void CEngine::Render()
{

}