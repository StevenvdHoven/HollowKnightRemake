#include "pch.h"
#include "GUI.h"
#include <iostream>
#include "GUIManager.h"

//bool GUI::MouseOnGUI{ false };
//Point2f GUI::m_MousePos{};
//std::vector<GUI*> GUI::m_AllGui{};
//std::map<std::string, TTF_Font*> GUI::m_Fonts{};



GUI::GUI():
	m_Manager{ nullptr },
	m_Hidden {false},
	m_Active {true},
	m_Interactable{true},
	m_Order{0},
	m_Parent{nullptr}
{
}

GUI::~GUI()
{
	if (m_Parent != nullptr) {
		m_Parent = nullptr;
	}

	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr)
			continue;

		m_Childeren[index]->OnDelete();
		delete m_Childeren[index];
		m_Childeren[index] = nullptr;
	}
	m_Childeren.clear();

	
}

void GUI::Draw()
{
}

void GUI::Update()
{
}

void GUI::CheckOnClickDown(const Point2f& mousepos)
{
}

void GUI::CheckOnClickUp(const Point2f& mousepos)
{
}

void GUI::OnKeyDown(const SDL_KeyboardEvent& e)
{
}

void GUI::OnKeyUp(const SDL_KeyboardEvent& e)
{
}

void GUI::DrawChilderen()
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr || m_Childeren[index]->IsHidden()) {
			continue;
		}
		m_Childeren[index]->Draw();
		m_Childeren[index]->DrawChilderen();
	}
}

void GUI::UpdateChilderen()
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr || !m_Childeren[index]->IsActive()) {
			continue;
		}
		m_Childeren[index]->Update();
		m_Childeren[index]->UpdateChilderen();
	}
}

void GUI::CheckOnClickDownChild(const Point2f& mousepos)
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr || !m_Childeren[index]->IsInteractable()) {
			continue;
		}
		m_Childeren[index]->CheckOnClickDown(m_Manager->GetMousePos());
		m_Childeren[index]->CheckOnClickDownChild(m_Manager->GetMousePos());
	}
}

void GUI::CheckOnClickUpChild(const Point2f& mousepos)
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr || !m_Childeren[index]->IsInteractable()) {
			continue;
		}
		m_Childeren[index]->CheckOnClickUp(m_Manager->GetMousePos());
		m_Childeren[index]->CheckOnClickUpChild(m_Manager->GetMousePos());
	}
}

void GUI::OnKeyDownChild(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr || !m_Childeren[index]->IsInteractable()) {
			continue;
		}
		m_Childeren[index]->OnKeyDown(e);
		m_Childeren[index]->OnKeyDownChild(e);
	}
}

void GUI::OnKeyUpChild(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr || !m_Childeren[index]->IsInteractable()) {
			continue;
		}
		m_Childeren[index]->OnKeyUp(e);
		m_Childeren[index]->OnKeyUpChild(e);
	}
}

void GUI::SetHiding(bool hide)
{
	m_Hidden = hide;
}

bool GUI::IsHidden()
{
	return m_Hidden;
}

void GUI::SetActive(bool active)
{
	m_Active = active;
	m_Hidden = !active;
	m_Interactable = active;
}

bool GUI::IsActive()
{
	return m_Active;
}

void GUI::SetInteractable(bool interactable)
{
	m_Interactable = interactable;
}

bool GUI::IsInteractable()
{
	return m_Interactable;
}

bool GUI::CheckOverUIChild(const Point2f& mousepos)
{
	for (int index{ 0 }; index < m_Childeren.size(); ++index)
	{
		if (m_Childeren[index] == nullptr || !m_Childeren[index]->IsActive()) {
			continue;
		}

		if (m_Childeren[index]->CheckOverUI(mousepos) && !m_Childeren[index]->IsHidden())
		{
			return true;
		}
	}
	return false;
}

bool GUI::CheckOverUI(const Point2f& mousepos)
{
	return false;
}

void GUI::OnDelete()
{
}

void GUI::OnParent(GUI* parent)
{
}

void GUI::AddChild(GUI* child)
{
	child->SetManager(m_Manager);

	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == nullptr) {
			m_Childeren[index] = child;
		}
		else if (m_Childeren[index] == child) 
		{
			return;
		}
	}
	m_Childeren.push_back(child);
}

void GUI::RemoveChild(GUI* child)
{
	std::vector<GUI*> childeren;
	for (int index{ 0 }; index < m_Childeren.size(); ++index) {
		if (m_Childeren[index] == child) {
			m_Childeren[index] = nullptr;
		}
		else {
			childeren.push_back(m_Childeren[index]);
		}
	}
	m_Childeren = childeren;
}

void GUI::SetParent(GUI* parent)
{
	if (parent == nullptr) {
		if (m_Parent != nullptr) {
			m_Parent->RemoveChild(this);
			m_Parent = nullptr;
		}
	}
	else {
		m_Parent = parent;
		m_Manager = parent->GetManager();
		m_Parent->AddChild(this);
	}

	OnParent(parent);
}

std::vector<GUI*> GUI::GetChilderen()
{
	return m_Childeren;
}

void GUI::SetOrder(int order)
{
	m_Order = order;
	m_Manager->Sort();
}

int GUI::GetOrder() const
{
	return m_Order;
}

void GUI::SetManager(GUIManager* manager)
{
	m_Manager = manager;
}

GUIManager* GUI::GetManager()
{
	return m_Manager;
}


