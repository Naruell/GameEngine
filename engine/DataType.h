#pragma once

struct Vector2D;
struct Vector2DInt;
struct Rect;

struct [[nodiscard]] Vector2D
{
    constexpr Vector2D() noexcept = default;
    explicit constexpr Vector2D(double value) noexcept : Vector2D{value, value} {};
    constexpr Vector2D(double x_value, double y_value) noexcept : x{ x_value }, y{ y_value } {};

    double x{ 0 };
    double y{ 0 };

    [[nodiscard]] double GetAngle() const noexcept;
    void SetAngle(double angleTo) noexcept;
    [[nodiscard]] double GetLength() const noexcept;
    void SetLength(double lengthTo) noexcept;

    constexpr Vector2D operator+(Vector2D v) const noexcept;
    constexpr Vector2D operator-(Vector2D v) const noexcept;
    constexpr Vector2D operator*(double scalar) const noexcept;
    constexpr Vector2D operator/(double scalar) const noexcept;
    constexpr Vector2D& operator+=(Vector2D v) noexcept;
    constexpr Vector2D& operator-=(Vector2D v) noexcept;
    constexpr Vector2D& operator*=(double scalar) noexcept;
    constexpr Vector2D& operator/=(double scalar) noexcept;

    [[nodiscard]] double Dot(Vector2D v) const noexcept;

    explicit operator Vector2DInt() const noexcept;
};

struct [[nodiscard]] Vector2DInt
{
    constexpr Vector2DInt() noexcept = default;
    explicit constexpr Vector2DInt(int value) noexcept : Vector2DInt(value, value) {};
    constexpr Vector2DInt(int x_value, int y_value) noexcept : x{ x_value }, y{ y_value } {};

    int x{ 0 };
    int y{ 0 };

    constexpr Vector2DInt operator+(Vector2DInt v) const noexcept;
    constexpr Vector2DInt operator-(Vector2DInt v) const noexcept;
    constexpr Vector2DInt operator-() const noexcept;
    constexpr Vector2DInt operator*(int scalar) const noexcept;
    constexpr Vector2DInt operator/(int scalar) const noexcept;
    constexpr Vector2DInt& operator+=(Vector2DInt v) noexcept;
    constexpr Vector2DInt& operator-=(Vector2DInt v) noexcept;
    constexpr bool operator==(Vector2DInt v) noexcept;
    constexpr bool operator!=(Vector2DInt v) noexcept;
    constexpr bool operator<(const Vector2DInt& v) const noexcept;

    explicit operator Vector2D() const noexcept;
};

struct [[nodiscard]] Rect
{
    Vector2D bottomLeft;
    Vector2D topRight;
};

#include "DataType.inl"

double GetDistanceBetweenDots(Vector2D v1, Vector2D v2);
double GetDistanceBetweenDotsSquared(Vector2D v1, Vector2D v2);
double GetDistanceBetweenDotAndLineSegment(Vector2D p, Vector2D v, Vector2D w);
double GetDistanceBetweenDotAndLine(Vector2D p, Vector2D v, Vector2D w);