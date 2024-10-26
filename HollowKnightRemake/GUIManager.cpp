#include "pch.h"
#include "GUIManager.h"
#include <algorithm>

GUIManager::GUIManager():
	m_MousePos{},
	m_MouseOnGUI{false}
{
	LoadFonts();
}

GUIManager::~GUIManager()
{
	CleanUp();
}

void GUIManager::LoadFonts()
{
	m_Fonts = std::map<std::string, TTF_Font*>
	{
		{arial_10, TTF_OpenFont("Fonts/arial.ttf",10)},
		{arial_14, TTF_OpenFont("Fonts/arial.ttf",14)},
		{arial_24, TTF_OpenFont("Fonts/arial.ttf",24)},
		{trajan_pro_regular_24, TTF_OpenFont("Fonts/TrajanProRegular.ttf",24)},
		{trajan_pro_regular_36, TTF_OpenFont("Fonts/TrajanProRegular.ttf",36)},
		{trajan_pro_regular_40, TTF_OpenFont("Fonts/TrajanProRegular.ttf",40)},
	};

}

void GUIManager::DrawUI()
{
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr || m_AllGui[index]->IsHidden()) {
			continue;
		}
		m_AllGui[index]->Draw();
		m_AllGui[index]->DrawChilderen();
	}
}

void GUIManager::UpdateUI()
{
	m_MouseOnGUI = false;

	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr || !m_AllGui[index]->IsActive()) {
			continue;
		}
		m_AllGui[index]->Update();
		m_AllGui[index]->UpdateChilderen();

		if (!m_AllGui[index]->IsHidden()) {

			bool mainResult{ m_AllGui[index]->CheckOverUI(m_MousePos) };
			bool childResult{ m_AllGui[index]->CheckOverUIChild(m_MousePos) };
			if (mainResult || childResult)
			{
				m_MouseOnGUI = true;
			}
		}
	}
}

void GUIManager::OnClickDown()
{
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr || !m_AllGui[index]->IsInteractable()) {
			continue;
		}
		m_AllGui[index]->CheckOnClickDown(m_MousePos);
		m_AllGui[index]->CheckOnClickDownChild(m_MousePos);
	}
}

void GUIManager::OnClickUp()
{
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr || !m_AllGui[index]->IsInteractable()) {
			continue;
		}
		m_AllGui[index]->CheckOnClickUp(m_MousePos);
		m_AllGui[index]->CheckOnClickUpChild(m_MousePos);
	}
}

void GUIManager::OnKeyDownUI(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr || !m_AllGui[index]->IsInteractable()) {
			continue;
		}
		m_AllGui[index]->OnKeyDown(e);
		m_AllGui[index]->OnKeyDownChild(e);
	}
}

void GUIManager::OnKeyUpUI(const SDL_KeyboardEvent& e)
{
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr || !m_AllGui[index]->IsInteractable()) {
			continue;
		}
		m_AllGui[index]->OnKeyUp(e);
		m_AllGui[index]->OnKeyUpChild(e);
	}
}

void GUIManager::AddGUI(GUI* pGui)
{
	pGui->SetManager(this);
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == nullptr) {
			m_AllGui[index] = pGui;
			return;
		}
	}
	m_AllGui.push_back(pGui);
	Sort();
}

void GUIManager::DeleteGUI(GUI* pGui)
{
	for (int index{ 0 }; index < m_AllGui.size(); ++index) {
		if (m_AllGui[index] == pGui) {
			delete m_AllGui[index];
			m_AllGui[index] = nullptr;
			return;
		}
	}
	Sort();
}

void GUIManager::CleanUp()
{
	std::map<std::string, TTF_Font*>::iterator it;

	for (it = m_Fonts.begin(); it != m_Fonts.end(); ++it) {
		TTF_CloseFont(m_Fonts[it->first]);
	}
	m_Fonts.clear();

	for (GUI* ui : m_AllGui) {
		if (ui == nullptr) {
			continue;
		}
		ui->OnDelete();
		delete ui;
	}
	m_AllGui.clear();
}

TTF_Font* GUIManager::GetFont(const std::string& fontName)
{
	return m_Fonts[fontName];
}

void GUIManager::SetMousePosition(const Point2f& position)
{
	float xScaler = 1920.f / 1280.f ;
	float yScaler = 1080.f / 800.f;

	m_MousePos = position;
	m_MousePos.x = m_MousePos.x * xScaler;
	m_MousePos.y = m_MousePos.y * yScaler;
}

Point2f GUIManager::GetMousePos()
{
	return m_MousePos;
}

bool GUIManager::IsMouseOnGui()
{
	return m_MouseOnGUI;
}

void GUIManager::Sort()
{
	std::sort(m_AllGui.begin(), m_AllGui.end(), [](const GUI* a, const GUI* b)
	{
			if (a == nullptr) return false;
			if (b == nullptr) return true;

			return a->GetOrder() < b->GetOrder();
	});
}
