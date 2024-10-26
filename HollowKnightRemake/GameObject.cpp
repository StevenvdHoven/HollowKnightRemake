#include "pch.h"
#include "utils.h"
#include "GameObject.h"
#include "MapObject.h"
#include "PolyCollider.h"
#include "RectCollider.h"
#include "PhyicsBody.h"
#include "SpriteRenderer.h"
#include "PhysicsEngine.h"
#include "Lift.h"
#include "KillZone.h"
#include "CameraZone.h"
#include "GameObjectManager.h"


GameObject::GameObject():
	m_Name{},
	m_Tag{},
	m_Transform{},
	m_Manager{nullptr}
{
}

GameObject::GameObject(const MapObject* object):
	m_Name{},
	m_Tag{object->GetTag()},
	m_Manager{nullptr}
{
	m_Transform.Position = Vector2f(object->GetPosition());
	m_Transform.Rotation = object->GetRotation();
	m_Transform.Scale = object->GetScale();

	if (object->GetIsCollider() && (object->GetTag() != "liftsmall" && object->GetTag() != "liftbig"))
	{
		Rectf bounds{ object->GetBounds() };
		Transform scaled;
		scaled.Scale = m_Transform.Scale;

		Transform rotated;
		rotated.Rotation = m_Transform.Rotation;

		Transform trans;
		trans.Position = -m_Transform.Position + Vector2f(-bounds.width * 0.5f, -bounds.height * 0.5f);

		std::vector<Point2f> poly{ utils::ApplyMatrix(trans,utils::GetPolygon(bounds)) };

		poly = utils::ApplyMatrix(scaled, poly);
		poly = utils::ApplyMatrix(rotated, poly);

		trans.Position = m_Transform.Position;
		poly = utils::ApplyMatrix(trans, poly);

		PolyCollider* collider{ new PolyCollider(poly, object->GetPosition()) };
		collider->m_Layer = Static;
		collider->m_IsTrigger = object->GetIsTrigger();

		if (object->GetTag() == "killzone")
		{
			AddComponent(new KillZone{});
		}
		else if (object->GetTag() == "camerazone" || 
			object->GetTag() == "camerazonex" || 
			object->GetTag() == "camerazoney" ||
			object->GetTag() == "camerazonec")
		{
			bool lockX{ true };
			bool lockY{ true };

			if (object->GetTag() == "camerazonex")
			{
				lockY = false;
			}
			else if (object->GetTag() == "camerazoney")
			{
				lockX = false;
			}
	

			CameraZone* zone{ new CameraZone{lockX, lockY,object->GetTag() == "camerazonec"} };
			AddComponent(zone);
		}

		AddComponent(collider);
	}
	else if (object->GetTag() == "liftsmall" || object->GetTag() == "liftbig")
	{
		Rectf bounds{ object->GetBounds() };

		RectCollider* collider{ new RectCollider(bounds.width,bounds.height,object->GetPosition()) };
		collider->m_Layer = Static;
		collider->m_IsTrigger = object->GetIsTrigger();

		AddComponent(collider);
		AddComponent(new Lift{ object->GetTag() == "liftsmall" });
	}

	if (object->GetTextureIndex() != -1) 
	{
		SpriteRenderer* renderer{ new SpriteRenderer(
			object->GetTextureIndex(),
			object->GetBounds(),
			object->GetSourceRect(),
			object->m_Order,
			object->GetIsBackground(),
			object->GetIsForeground()
		) };

		renderer->SetTint(object->GetTint());

		AddComponent(renderer);
	}

	
}

GameObject::~GameObject()
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		m_Components[index]->OnDelete();
		delete m_Components[index];
		m_Components[index] = nullptr;
	}
	m_Components.clear();
}

void GameObject::Init()
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr) {
			m_Components[index]->Init();
		}
	}
}

void GameObject::Start()
{
	
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr) {
			m_Components[index]->Start();
		}
	}
	
}

void GameObject::Update(float elapsedTime)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->Update(elapsedTime);
		}
	}
}



void GameObject::Draw()
{
	utils::SetColor(Color4f(0, 1, 0, 1));
	utils::DrawPoint(Point2f(m_Transform.Position), 10);
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->Draw();
		}
	}
}

void GameObject::KeyDownEvent(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->KeyDownEvent(e);
		}
	}
}

void GameObject::KeyUpEvent(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->KeyUpEvent(e);
		}
	}
}

void GameObject::MouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->MouseMotionEvent(e);
		}
	}
}

void GameObject::MouseDownEvent(const SDL_MouseButtonEvent& e)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->MouseDownEvent(e);
		}
	}
}

void GameObject::MouseUpEvent(const SDL_MouseButtonEvent& e)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->MouseUpEvent(e);
		}
	}
}

void GameObject::OnCollision(Collider* other)
{
	
}

void GameObject::OnCollisionEnter(Collider* collider)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->OnCollisionEnter(collider);
		}
	}
}

void GameObject::OnCollisionStay(Collider* collider)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->OnCollisionStay(collider);
		}
	}
}

void GameObject::OnCollisionExit(Collider* collider)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->OnCollisionExit(collider);
		}
	}
}

void GameObject::OnTriggerEnter(Collider* collider)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->OnTriggerEnter(collider);
		}
	}
}

void GameObject::OnTriggerStay(Collider* collider)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr && m_Components[index]->IsEnabled()) {
			m_Components[index]->OnTriggerStay(collider);
		}
	}
}

void GameObject::SetManager(GameObjectManager* manager)
{
	m_Manager = manager;
}

void GameObject::SetPosition(const Point2f& position, bool force)
{
	Point2f lastpos{ Point2f(m_Transform.Position) };
	float distance{ utils::GetDistance(lastpos,position) };
	if (distance > 2 || force) {
		m_Transform.Position = Vector2f(position);
	}
}

Point2f GameObject::GetPosition()
{
	return Point2f(m_Transform.Position);
}

void GameObject::AddComponent(Component* component)
{
	component->SetHolder(this);
	
	for (int index{ 0 }; index < m_Components.size(); ++index) 
	{
		if (m_Components[index] == nullptr) 
		{
			m_Components[index] = component;
			return;
		}
	}
	m_Components.push_back(component);
}

void GameObject::DeleteComponent(const Component* component)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] == component) {
			delete m_Components[index];
			m_Components[index] = nullptr;
		}
	}
}

void GameObject::AddGameObject(GameObject* gameObject)
{
	m_Manager->AddObject(gameObject);
}

void GameObject::OnTriggerExit(Collider* collider)
{
	for (int index{ 0 }; index < m_Components.size(); ++index) {
		if (m_Components[index] != nullptr) {
			m_Components[index]->OnTriggerExit(collider);
		}
	}
}

void GameObject::DeleteGameObject(const GameObject* gameObject)
{
	m_Manager->DeleteObject(gameObject);
}

void GameObject::AddToDontDestory(GameObject* gameObject)
{
}

Vector2f GameObject::GetFrontVector() const
{
	return utils::ApplyMatrix(m_Transform, Vector2f(1, 0));
}

Vector2f GameObject::GetUpVector() const 
{
	return utils::ApplyMatrix(m_Transform, Vector2f(0, 1));
}

PlayerDataManager* GameObject::GetPlayerDataManager()
{
	return m_Manager->GetPlayerDataManager();
}

LevelManager* GameObject::GetLevelManager()
{
	return m_Manager->GetLevelManager();
}

SpriteManager* GameObject::GetSpriteManager()
{
	return m_Manager->GetSpriteManager();
}

PhysicsEngine* GameObject::GetPhysicsEngine()
{
	return m_Manager->GetPhysicsEngine();
}

TextureManager* GameObject::GetTextureManager()
{
	return m_Manager->GetTextureManager();
}

CameraManager* GameObject::GetCameraManager()
{
	return m_Manager->GetCameraManager();
}

GameObjectManager* GameObject::GetGameObjectManager()
{
	return m_Manager;
}

AudioManager* GameObject::GetAudioManager()
{
	return m_Manager->GetAudioManager();
}

GUIManager* GameObject::GetGUIManager()
{
	return m_Manager->GetGUIManager();
}


