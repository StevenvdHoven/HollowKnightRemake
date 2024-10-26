#pragma once
#include <vector>
#include <map>
#include "GUI.h"

#define arial_10 "arial_10"
#define arial_14 "arial_14"
#define arial_24 "arial_24"

#define trajan_pro_regular_24 "trajan_pro_regular_24"
#define trajan_pro_regular_36 "trajan_pro_regular_36"
#define trajan_pro_regular_40 "trajan_pro_regular_40"

#define white Color4f{ 1,1,1,1 }
#define black Color4f{ 0,0,0,1 }

class GUIManager final
{
public:
	GUIManager();
	~GUIManager();

	void LoadFonts();
	void DrawUI();
	void UpdateUI();
	void OnClickDown();
	void OnClickUp();
	void OnKeyDownUI(const SDL_KeyboardEvent& e);
	void OnKeyUpUI(const SDL_KeyboardEvent& e);
	void AddGUI(GUI* pGui);
	void DeleteGUI(GUI* pGui);
	void CleanUp();

	TTF_Font* GetFont(const std::string& fontName);

	void SetMousePosition(const Point2f& position);
	Point2f GetMousePos();
	bool IsMouseOnGui();

	void Sort();
	
private:
	Point2f m_MousePos;
	bool m_MouseOnGUI;

	std::vector<GUI*> m_AllGui;
	std::map<std::string, TTF_Font*> m_Fonts;
	
};

