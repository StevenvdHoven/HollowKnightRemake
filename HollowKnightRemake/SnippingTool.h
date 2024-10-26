#pragma once
#include <vector>
#include <functional>

class MapObject;
class Texture;
class TextureManager;
class GUIManager;
class SnippingTool final
{
public:
	SnippingTool(const Rectf& viewPort,TextureManager* manager, GUIManager* guiManager, std::function<void(MapObject*)> func);
	~SnippingTool();

	void OnClick(const Point2f& mousePos);
	void OnMouseMotion(const Point2f& mousePos);
	void OnClickUp(const Point2f& mousePos);

	void Draw();
	
	bool IsMouseOverSnippingTool(const Point2f& mousePos);

private:
	void LoadOptions();

	void CloseTexture();
	void Clear();
	void Export();
	int	CheckForOptionClick(const Point2f& mousePos);
	bool CheckForButtonClick(const Point2f& mousePos);
	bool PointInRect(const Rectf& rect, const Point2f& point);

	void DrawOptions();
	void DrawButtons();
	void DrawPanel();
	void DrawTexture();
	void DrawCuttingRect();

	Rectf CaculateBounds();

	const Rectf m_ViewPort;

	std::function<void(MapObject*)> m_ExportCallBack;

	Point2f m_Position;

	bool m_Dragging;
	Point2f m_StartSnipPosition;
	Point2f m_EndSnipPosition;

	Rectf m_CurrentBounds;
	Texture* m_CurrentLoadedTexture;
	int m_CurrentTextureIndex;
	const float m_PanelSpacing;

	TextureManager* m_TextureManager;
	GUIManager* m_GUIManager;

	std::vector<std::string> m_Options;
	std::vector<std::string> m_EffectOptions;


	TTF_Font* m_Font;

	// Icons
	Texture* m_CuttingIcon;
	Texture* m_EraseIcon;
	Texture* m_ListIcon;

};

