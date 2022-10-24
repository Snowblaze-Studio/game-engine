#pragma once

#ifndef CORE_HPP // include guard
#define CORE_HPP

class Vector2
{
public:
    float x;
    float y;

public:
    Vector2() : x(0), y(0) {}

    Vector2(const float x, const float y) : x(x), y(y) {}

    const static Vector2 GRAVITY;
    const static Vector2 UP;

    void clear()
    {
        x = y = 0;
    }

    void invert()
    {
        x = -x;
        y = -y;
    }

    void addScaledVector(const Vector2& vector, float scale)
    {
        x += vector.x * scale;
        y += vector.y * scale;
    }

    void operator+=(const Vector2& vector)
    {
        x += vector.x;
        y += vector.y;
    }

    Vector2 operator+(const Vector2& vector) const
    {
        return Vector2(x + vector.x, y + vector.y);
    }

    void operator-=(const Vector2& vector)
    {
        x -= vector.x;
        y -= vector.y;
    }

    Vector2 operator-(const Vector2& vector) const
    {
        return Vector2(x - vector.x, y - vector.y);
    }

    void operator*=(const float value)
    {
        x *= value;
        y *= value;
    }

    Vector2 operator*(const float value) const
    {
        return Vector2(x * value, y * value);
    }

    float operator*(const Vector2& vector) const
    {
        return x * vector.x + y * vector.y;
    }
};

#endif
