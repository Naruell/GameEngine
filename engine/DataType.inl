#pragma once
#include <cmath>

inline double Vector2D::GetAngle() const noexcept
{
    return std::atan2(y, x);
}

inline void Vector2D::SetAngle(double angleTo) noexcept
{
    x = GetLength() * std::cos(angleTo);
    y = GetLength() * std::sin(angleTo);
}

inline double Vector2D::GetLength() const noexcept
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

inline void Vector2D::SetLength(double length) noexcept
{
    x = length * std::cos(GetAngle());
    y = length * std::sin(GetAngle());
}

inline constexpr Vector2D Vector2D::operator+(Vector2D v) const noexcept
{
    return Vector2D{ x + v.x, y + v.y };
}

inline constexpr Vector2D Vector2D::operator-(Vector2D v) const noexcept
{
    return Vector2D{ x - v.x, y - v.y };
}

inline constexpr Vector2D Vector2D::operator*(double scalar) const noexcept
{
    return Vector2D{ x * scalar, y * scalar };
}

inline constexpr Vector2D Vector2D::operator/(double scalar) const noexcept
{
    return Vector2D{ x / scalar, y / scalar };
}

inline constexpr Vector2D& Vector2D::operator+=(Vector2D v) noexcept
{
    x += v.x;
    y += v.y;
    return *this;
}

inline constexpr Vector2D& Vector2D::operator-=(Vector2D v) noexcept
{
    x -= v.x;
    y -= v.y;
    return *this;
}

inline constexpr Vector2D& Vector2D::operator*=(double scalar) noexcept
{
    x *= scalar;
    y *= scalar;
    return *this;
}

inline constexpr Vector2D& Vector2D::operator/=(double scalar) noexcept
{
    x /= scalar;
    y /= scalar;
    return *this;
}

inline Vector2D::operator Vector2DInt() const noexcept
{
    return Vector2DInt{ static_cast<int>(x), static_cast<int>(y) };
}

inline constexpr Vector2DInt Vector2DInt::operator+(Vector2DInt v) const noexcept
{
    return Vector2DInt{ x + v.x, y + v.y };
}

inline constexpr Vector2DInt Vector2DInt::operator-(Vector2DInt v) const noexcept
{
    return Vector2DInt{ x - v.x, y - v.y };
}

inline constexpr Vector2DInt Vector2DInt::operator-() const noexcept
{
    return Vector2DInt{ -x,-y };
}

inline constexpr Vector2DInt Vector2DInt::operator*(int scalar) const noexcept
{
    return Vector2DInt{ x * scalar, y * scalar };
}

inline constexpr Vector2DInt Vector2DInt::operator/(int scalar) const noexcept
{
    return Vector2DInt{ x / scalar, y / scalar };
}

inline constexpr Vector2DInt& Vector2DInt::operator+=(Vector2DInt v) noexcept
{
    x += v.x;
    y += v.y;
    return *this;
}
inline constexpr Vector2DInt& Vector2DInt::operator-=(Vector2DInt v) noexcept
{
    x -= v.x;
    y -= v.y;
    return *this;
}

inline constexpr bool Vector2DInt::operator==(Vector2DInt v) noexcept
{
    return (x == v.x && y == v.y);
}

inline constexpr bool Vector2DInt::operator!=(Vector2DInt v) noexcept
{
    return (x != v.x || y != v.y);
}

inline constexpr bool Vector2DInt::operator<(const Vector2DInt& v) const noexcept
{
    if (y < v.y) { return true; }
    else if (y > v.y) { return false; }
    else if(y == v.y && x < v.x) { return true;	}
    else { return false; }
}

inline double Vector2D::Dot(Vector2D v) const noexcept
{
    return x * v.x + y * v.y;
}

inline Vector2DInt::operator Vector2D() const noexcept
{
    return Vector2D{ static_cast<double>(x), static_cast<double>(y) };
}