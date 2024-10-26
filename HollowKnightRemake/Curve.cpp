#include "pch.h"
#include "Curve.h"

Curve::Curve():
    Orgin{0,1},
    ControlPoint1{0,1},
    ControlPoint2{1,1},
    End{1,1},
    BaseValue{ 1 }
{
}

Curve::Curve(Vector2f orgin, Vector2f c1, Vector2f c2, Vector2f end, float baseValue):
    Orgin{orgin},
    ControlPoint1{c1},
    ControlPoint2{c2},
    End{end},
    BaseValue{baseValue}
{
}

Point2f Curve::GetPointInCurve(float percentile) const
{
    return Point2f(powf((1 - percentile), 3) * Orgin 
        + 3 * powf((1 - percentile), 2) * percentile * ControlPoint1 
        + 3 * (1 - percentile) * powf(percentile, 2) * ControlPoint2 
        + powf(percentile, 3) * End);
}

float Curve::GetValue(float x) const
{
    return GetPointInCurve(x).y * BaseValue;
}
