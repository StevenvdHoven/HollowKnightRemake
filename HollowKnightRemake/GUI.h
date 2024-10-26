#pragma once
#include <vector>
#include <map>



class GUIManager;
class GUI
{
public:
	GUI();
	virtual ~GUI();

	virtual void Draw();
	virtual void Update();
	virtual void CheckOnClickDown(const Point2f& mousepos);
	virtual void CheckOnClickUp(const Point2f& mousepos);

	virtual void OnKeyDown(const SDL_KeyboardEvent& e);
	virtual void OnKeyUp(const SDL_KeyboardEvent& e);

	virtual bool CheckOverUI(const Point2f& mousepos);

	virtual void OnDelete();
	virtual void OnParent(GUI* parent);

	void DrawChilderen();
	void UpdateChilderen();
	void CheckOnClickDownChild(const Point2f& mousepos);
	void CheckOnClickUpChild(const Point2f& mousepos);
	void OnKeyDownChild(const SDL_KeyboardEvent& e);
	void OnKeyUpChild(const SDL_KeyboardEvent& e);

	void SetHiding(bool hide);
	bool IsHidden();

	void SetActive(bool active);
	bool IsActive();

	void SetInteractable(bool interactable);
	bool IsInteractable();
	
	bool CheckOverUIChild(const Point2f& mousepos);

	void AddChild(GUI* child);
	void RemoveChild(GUI* child);
	void SetParent(GUI* parent);
	std::vector<GUI*> GetChilderen();

	void SetOrder(int order);
	int GetOrder() const;

	void SetManager(GUIManager* manager);
	GUIManager* GetManager();

protected:
	GUIManager* m_Manager;

private:
	bool m_Hidden;
	bool m_Active;
	bool m_Interactable;

	int m_Order;

	GUI* m_Parent;
	std::vector<GUI*> m_Childeren;

	

};

