#include "DataType.h"

#include <cmath>
#include <algorithm>

double GetDistanceBetweenDots(Vector2D v1, Vector2D v2)
{
    const double x_diff = v1.x - v2.x;
    const double y_diff = v1.y - v2.y;
    return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

double GetDistanceBetweenDotsSquared(Vector2D v1, Vector2D v2)
{
    const double x_diff = v1.x - v2.x;
    const double y_diff = v1.y - v2.y;
    return x_diff * x_diff + y_diff * y_diff;
}

double GetDistanceBetweenDotAndLineSegment(Vector2D p, Vector2D v, Vector2D w)
{
    // Return minimum distance between line segment vw and point p
    const double l2 = GetDistanceBetweenDotsSquared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
    if (l2 == 0.0) return GetDistanceBetweenDots(p, v);   // v == w case
    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const double t = std::max(0.0, std::min(1.0, (p - v).Dot(w - v) / l2));
    const Vector2D projection = v + (w - v) * t;  // Projection falls on the segment
    return GetDistanceBetweenDots(p, projection);
}

double GetDistanceBetweenDotAndLine(Vector2D p, Vector2D v, Vector2D w)
{
    double area = abs((v.x - p.x) * (w.y - p.y) - (v.y - p.y) * (w.x - p.x));
    double AB = sqrt(pow(v.x - w.x, 2) + pow(v.y - w.y, 2));
    return (area / AB);
}
