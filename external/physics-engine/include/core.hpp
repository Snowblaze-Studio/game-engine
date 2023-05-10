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

    void operator*=(const float value)
    {
        x *= value;
        y *= value;
    }
};

#endif
