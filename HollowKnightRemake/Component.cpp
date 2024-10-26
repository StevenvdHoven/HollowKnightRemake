#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component():
	m_Holder{ nullptr },
	m_IsEnabled{ true }
{
}

Component::~Component()
{
	m_Holder = nullptr;
}

void Component::Init()
{
}

void Component::Start()
{
}

void Component::Update(float elapsedSec)
{
}

void Component::Draw() const
{
}

void Component::KeyDownEvent(const SDL_KeyboardEvent& e)
{
}

void Component::KeyUpEvent(const SDL_KeyboardEvent& e)
{
}

void Component::MouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

void Component::MouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void Component::MouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

void Component::OnCollisionEnter(Collider* collider)
{
}

void Component::OnCollisionStay(Collider* collider)
{
}

void Component::OnCollisionExit(Collider* collider)
{
}

void Component::OnTriggerEnter(Collider* collider)
{
}

void Component::OnTriggerStay(Collider* collider)
{
}

void Component::OnTriggerExit(Collider* collider)
{
}

void Component::OnDelete()
{
}

GameObject* Component::GetGameObject() const
{
	return m_Holder;
}

void Component::SetHolder(GameObject* object)
{
	m_Holder = object;
}

PlayerDataManager* Component::GetPlayerDataManager()
{
	return m_Holder->GetPlayerDataManager();
}

LevelManager* Component::GetLevelManager()
{
	return m_Holder->GetLevelManager();
}

SpriteManager* Component::GetSpriteManager()
{
	return m_Holder->GetSpriteManager();
}

PhysicsEngine* Component::GetPhysicsEngine()
{
	return m_Holder->GetPhysicsEngine();
}

TextureManager* Component::GetTextureManager()
{
	return m_Holder->GetTextureManager();
}

CameraManager* Component::GetCameraManager()
{
	return m_Holder->GetCameraManager();
}

AudioManager* Component::GetAudioManager()
{
	return m_Holder->GetAudioManager();
}

GUIManager* Component::GetGUIManager()
{
	return m_Holder->GetGUIManager();
}

GameObjectManager* Component::GetGameObjectManager()
{
	return m_Holder->GetGameObjectManager();
}

void Component::SetEnabled(bool enabled)
{
	m_IsEnabled = enabled;
}

bool Component::IsEnabled()
{
	return m_IsEnabled;
}


