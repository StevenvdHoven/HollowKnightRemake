#pragma once
#include "utils.h"

#define curve_default Curve{Vector2f{0,1}, Vector2f{0,1}, Vector2f{1,1}, Vector2f{1,1}, 1}
#define curve_default_0 Curve{Vector2f{0,0}, Vector2f{0,0}, Vector2f{1,0}, Vector2f{1,0}, 1}

#define curve_linear Curve{Vector2f{1,1}, Vector2f{.5f,.5}, Vector2f{.5f,.5}, Vector2f{0,0}, 1}
#define cruve_linear_Down Curve{Vector2f{0,0}, Vector2f{.5f,.5}, Vector2f{.5f,.5}, Vector2f{1,1}, 1}

#define curve_slope_up Curve{Vector2f{0,0}, Vector2f{0.6f,0}, Vector2f{1,.3f}, Vector2f{1,1}, 1}
#define curve_slope_up_reversed Curve{Vector2f{0,0}, Vector2f{0,.7}, Vector2f{0.3f,1}, Vector2f{1,1}, 1}
#define curve_slope_down Curve{Vector2f{0,1}, Vector2f{0,1}, Vector2f{1, 1}, Vector2f{1,0}, 1}
#define curve_slope_down_reversed Curve{Vector2f{0,1}, Vector2f{0,1}, Vector2f{0,0}, Vector2f{0,1}, 1}

#define curve_smooth Curve{Vector2f{0,0}, Vector2f{.5f,0}, Vector2f{.5f,1}, Vector2f{1,1}, 1}

#define curve_s_up_shape Curve{Vector2f{0,0}, Vector2f{1,0}, Vector2f{0,1}, Vector2f{1,1}, 1}
#define curve_s_up_shape_reversed Curve{Vector2f{0,0}, Vector2f{0,1}, Vector2f{1,0}, Vector2f{1,1}, 1}
#define curve_s_down_shape Curve{Vector2f{0,1}, Vector2f{1,1}, Vector2f{0,0}, Vector2f{1,1}, 1}
#define curve_s_down_shape_reversed Curve{Vector2f{0,0}, Vector2f{1,1}, Vector2f{1,0}, Vector2f{1,1}, 1}

#define curve_half_circle Curve{Vector2f{0,0}, Vector2f{0,.66f}, Vector2f{0,.66f}, Vector2f{1,0}, 1}

struct Curve
{
	explicit Curve();
	explicit Curve(Vector2f orgin, Vector2f c1, Vector2f c2, Vector2f end, float baseValue = 1);

	Vector2f Orgin;
	Vector2f ControlPoint1;
	Vector2f ControlPoint2;
	Vector2f End;

	float BaseValue;

	Point2f GetPointInCurve(float percentile) const;
	float GetValue(float x) const;
};

