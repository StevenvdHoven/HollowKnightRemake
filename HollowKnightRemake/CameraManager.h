#pragma once
#include "Vector2f.h"
#include "Transform.h"

class CameraZone;
class CameraManager final
{
public:
	CameraManager(const Rectf& viewport);

	void SetCameraPosition(const Point2f& pos, bool force = false);
	void SetCameraRotation(float rot);
	void SetZoom(float zoom);

	Point2f GetCameraPosition();
	float GetCameraRotation();
	float GetZoom();
	Transform GetTransform();

	void SetZone(CameraZone* newZone);
	void RemoveZone(CameraZone* zone);

private:
	CameraZone* m_CurrentZone;

	Vector2f m_CameraPosition;
	float m_CameraRotation;
	float m_Scale;

	Rectf m_ViewPort;
};

