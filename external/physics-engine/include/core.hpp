#pragma once
#include <math.h>

#ifndef CORE_HPP // include guard
#define CORE_HPP

class Vector3
{
public:
    float x;
    float y;
    float z;

public:
    Vector3() : x(0), y(0), z(0) {}

    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    const static Vector3 GRAVITY;
    const static Vector3 UP;

    void clear()
    {
        x = y = z = 0;
    }

    void invert()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    void addScaledVector(const Vector3& vector, float scale)
    {
        x += vector.x * scale;
        y += vector.y * scale;
        z += vector.z * scale;
    }

    void operator+=(const Vector3& vector)
    {
        x += vector.x;
        y += vector.y;
        z += vector.z;
    }

    Vector3 operator+(const Vector3& vector) const
    {
        return Vector3(x + vector.x, y + vector.y, z + vector.z);
    }

    void operator-=(const Vector3& vector)
    {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
    }

    Vector3 operator-(const Vector3& vector) const
    {
        return Vector3(x - vector.x, y - vector.y, z - vector.z);
    }

    void operator*=(const float value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    Vector3 operator*(const float value) const
    {
        return Vector3(x * value, y * value, z * value);
    }

    float operator*(const Vector3& vector) const
    {
        return x * vector.x + y * vector.y + z * vector.z;
    }
};

class Quaternion
{
public:
    union {
        struct {
            float x;
            float y;
            float z;
            float w;
        };

        float data[4];
    };

    Quaternion() : x(0), y(0), z(0), w(1) {}

    Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

    void normalize()
    {
        float det = x * x + y * y + z * z + w * w;
        if (det == 0.0f)
        {
            w = 1.0f;
            return;
        }

        float invDet = 1 / sqrt(det);
        x *= invDet;
        y *= invDet;
        z *= invDet;
        w *= invDet;
    }

    void operator*=(const Quaternion& multiplier)
    {
        Quaternion q = *this;
        x = q.w * multiplier.x + q.x * multiplier.w + q.y * multiplier.z - q.z * multiplier.y;
        y = q.w * multiplier.y + q.y * multiplier.w + q.z * multiplier.x - q.x * multiplier.z;
        z = q.w * multiplier.z + q.z * multiplier.w + q.x * multiplier.y - q.y * multiplier.x;
        w = q.w * multiplier.w - q.x * multiplier.x - q.y * multiplier.y - q.z * multiplier.z;
    }

    void addScaledVector(const Vector3& vector, float scale)
    {
        Quaternion q(0, vector.x * scale, vector.y * scale, vector.z * scale);
        q *= *this;
        x += q.x * 0.5f;
        y += q.y * 0.5f;
        z += q.z * 0.5f;
        w += q.w * 0.5f;
    }
};

class Matrix3x3
{
public:
    float data[9];

    Matrix3x3()
    {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 0;
        data[6] = 0;
        data[7] = 0;
        data[8] = 0;
    }

    Matrix3x3(const Vector3& column0, const Vector3& column1, const Vector3& column2)
    {
        data[0] = column0.x;
        data[1] = column1.x;
        data[2] = column2.x;
        data[3] = column0.y;
        data[4] = column1.y;
        data[5] = column2.y;
        data[6] = column0.z;
        data[7] = column1.z;
        data[8] = column2.z;
    }

    Matrix3x3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8)
    {
        data[0] = c0;
        data[1] = c1;
        data[2] = c2;
        data[3] = c3;
        data[4] = c4;
        data[5] = c5;
        data[6] = c6;
        data[7] = c7;
        data[8] = c8;
    }

    Vector3 operator*(const Vector3& vector) const
    {
        return Vector3(
            vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
            vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
            vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
        );
    }

    Vector3 transform(const Vector3& vector) const
    {
        return (*this) * vector;
    }

    Matrix3x3 operator*(const Matrix3x3& o) const
    {
        return Matrix3x3(
            data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
            data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
            data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],
            data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
            data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
            data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],
            data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
            data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
            data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
        );
    }

    void operator*=(const Matrix3x3& o)
    {
        *this = *this * o;
    }

    void setInverse(const Matrix3x3& m)
    {
        float t4 = m.data[0] * m.data[4];
        float t6 = m.data[0] * m.data[5];
        float t8 = m.data[1] * m.data[3];
        float t10 = m.data[2] * m.data[3];
        float t12 = m.data[1] * m.data[6];
        float t14 = m.data[2] * m.data[6];
        // Calculate the determinant
        float t16 = (t4 * m.data[8] - t6 * m.data[7] - t8 * m.data[8] +
                       t10 * m.data[7] + t12 * m.data[5] - t14 * m.data[4]);
        // Make sure the determinant is non-zero.
        if (t16 == 0.0f) return;
        float t17 = 1 / t16;
        data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * t17;
        data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * t17;
        data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * t17;
        data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * t17;
        data[4] = (m.data[0] * m.data[8] - t14) * t17;
        data[5] = -(t6 - t10) * t17;
        data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * t17;
        data[7] = -(m.data[0] * m.data[7] - t12) * t17;
        data[8] = (t4 - t8) * t17;
    }

    Matrix3x3 inverse() const
    {
        Matrix3x3 result;
        result.setInverse(*this);
        return result;
    }

    void invert()
    {
        setInverse(*this);
    }

    void setTranspose(const Matrix3x3& m)
    {
        data[0] = m.data[0];
        data[1] = m.data[3];
        data[2] = m.data[6];
        data[3] = m.data[1];
        data[4] = m.data[4];
        data[5] = m.data[7];
        data[6] = m.data[2];
        data[7] = m.data[5];
        data[8] = m.data[8];
    }

    Matrix3x3 transpose() const
    {
        Matrix3x3 result;
        result.setTranspose(*this);
        return result;
    }

    void setOrientation(const Quaternion& q)
    {
        data[0] = 1 - (2 * q.y * q.y + 2 * q.z * q.z);
        data[1] = 2 * q.x * q.y + 2 * q.z * q.w;
        data[2] = 2 * q.x * q.z - 2 * q.y * q.w;
        data[3] = 2 * q.x * q.y - 2 * q.z * q.w;
        data[4] = 1 - (2 * q.x * q.x + 2 * q.z * q.z);
        data[5] = 2 * q.y * q.z + 2 * q.x * q.w;
        data[6] = 2 * q.x * q.z + 2 * q.y * q.w;
        data[7] = 2 * q.y * q.z - 2 * q.x * q.w;
        data[8] = 1 - (2 * q.x * q.x + 2 * q.y * q.y);
    }
};

class Matrix3x4
{
public:
    float data[12];

    Matrix3x4()
    {
        data[0] = 1;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 1;
        data[6] = 0;
        data[7] = 0;
        data[8] = 0;
        data[9] = 0;
        data[10] = 1;
        data[11] = 0;
    }

    Matrix3x4(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8, float c9, float c10, float c11)
    {
        data[0] = c0;
        data[1] = c1;
        data[2] = c2;
        data[3] = c3;
        data[4] = c4;
        data[5] = c5;
        data[6] = c6;
        data[7] = c7;
        data[8] = c8;
        data[9] = c9;
        data[10] = c10;
        data[11] = c11;
    }

    Vector3 operator*(const Vector3& vector) const
    {
        return Vector3(
            vector.x * data[0] + vector.y * data[1] + vector.z * data[2] + data[3],
            vector.x * data[4] + vector.y * data[5] + vector.z * data[6] + data[7],
            vector.x * data[8] + vector.y * data[9] + vector.z * data[10] + data[11]
        );
    }

    Vector3 transform(const Vector3& vector) const
    {
        return (*this) * vector;
    }

    Matrix3x4 operator*(const Matrix3x4& o) const
    {
        return Matrix3x4(
            data[0] * o.data[0] + data[1] * o.data[4] + data[2] * o.data[8],
            data[0] * o.data[1] + data[1] * o.data[5] + data[2] * o.data[9],
            data[0] * o.data[2] + data[1] * o.data[6] + data[2] * o.data[10],
            data[0] * o.data[3] + data[1] * o.data[7] + data[2] * o.data[11] + data[3],
            data[4] * o.data[0] + data[5] * o.data[4] + data[6] * o.data[8],
            data[4] * o.data[1] + data[5] * o.data[5] + data[6] * o.data[9],
            data[4] * o.data[2] + data[5] * o.data[6] + data[6] * o.data[10],
            data[4] * o.data[3] + data[5] * o.data[7] + data[6] * o.data[11] + data[7],
            data[8] * o.data[0] + data[9] * o.data[4] + data[10] * o.data[8],
            data[8] * o.data[1] + data[9] * o.data[5] + data[10] * o.data[9],
            data[8] * o.data[2] + data[9] * o.data[6] + data[10] * o.data[10],
            data[8] * o.data[3] + data[9] * o.data[7] + data[10] * o.data[11] + data[11]
        );
    }

    float getDeterminant() const
    {
        return data[0] * data[5] * data[10]
            - data[0] * data[6] * data[9]
            - data[1] * data[4] * data[10]
            + data[1] * data[6] * data[8]
            + data[2] * data[4] * data[9]
            - data[2] * data[5] * data[8];
    }

    void setInverse(const Matrix3x4& m)
    {
        float det = getDeterminant();

        if (det == 0.0f) return;

        float invDet = 1 / det;

        data[0] = (m.data[5] * m.data[10] - m.data[6] * m.data[9]) * invDet;
        data[1] = (m.data[2] * m.data[9] - m.data[1] * m.data[10]) * invDet;
        data[2] = (m.data[1] * m.data[6] - m.data[2] * m.data[5]) * invDet;
        data[3] = (m.data[3] * m.data[6] * m.data[9] +
                   m.data[2] * m.data[5] * m.data[11] +
                   m.data[1] * m.data[7] * m.data[10] -
                   m.data[1] * m.data[6] * m.data[11] -
                   m.data[2] * m.data[7] * m.data[9] -
                   m.data[3] * m.data[5] * m.data[10]) * invDet;

        data[4] = (m.data[6] * m.data[8] - m.data[4] * m.data[10]) * invDet;
        data[5] = (m.data[0] * m.data[10] - m.data[2] * m.data[8]) * invDet;
        data[6] = (m.data[2] * m.data[4] - m.data[0] * m.data[6]) * invDet;
        data[7] = (m.data[3] * m.data[4] * m.data[10] +
                   m.data[2] * m.data[7] * m.data[8] +
                   m.data[0] * m.data[6] * m.data[11] -
                   m.data[3] * m.data[6] * m.data[8] -
                   m.data[2] * m.data[4] * m.data[11] -
                   m.data[0] * m.data[7] * m.data[10]) * invDet;

        data[8] = (m.data[4] * m.data[9] - m.data[5] * m.data[8]) * invDet;
        data[9] = (m.data[1] * m.data[8] - m.data[0] * m.data[9]) * invDet;
        data[10] = (m.data[0] * m.data[5] - m.data[1] * m.data[4]) * invDet;
        data[11] = (m.data[3] * m.data[5] * m.data[8] +
                    m.data[1] * m.data[4] * m.data[11] +
                    m.data[0] * m.data[7] * m.data[9] -
                    m.data[3] * m.data[4] * m.data[9] -
                    m.data[0] * m.data[5] * m.data[11] -
                    m.data[1] * m.data[7] * m.data[8]) * invDet;
    }

    Matrix3x4 inverse() const
    {
        Matrix3x4 result;
        result.setInverse(*this);
        return result;
    }

    void invert()
    {
        setInverse(*this);
    }

    void setOrientationAndPos(const Quaternion& q, const Vector3& pos)
    {
        data[0] = 1 - (2 * q.y * q.y + 2 * q.z * q.z);
        data[1] = 2 * q.x * q.y + 2 * q.z * q.w;
        data[2] = 2 * q.x * q.z - 2 * q.y * q.w;
        data[3] = pos.x;

        data[4] = 2 * q.x * q.y - 2 * q.z * q.w;
        data[5] = 1 - (2 * q.x * q.x + 2 * q.z * q.z);
        data[6] = 2 * q.y * q.z + 2 * q.x * q.w;
        data[7] = pos.y;

        data[8] = 2 * q.x * q.z + 2 * q.y * q.w;
        data[9] = 2 * q.y * q.z - 2 * q.x * q.w;
        data[10] = 1 - (2 * q.x * q.x + 2 * q.y * q.y);
        data[11] = pos.z;
    }

    Vector3 transformInverse(const Vector3& vector) const
    {
        Vector3 tmp = vector;
        tmp.x -= data[3];
        tmp.y -= data[7];
        tmp.z -= data[11];
        return Vector3(
            tmp.x * data[0] + tmp.y * data[4] + tmp.z * data[8],
            tmp.x * data[1] + tmp.y * data[5] + tmp.z * data[9],
            tmp.x * data[2] + tmp.y * data[6] + tmp.z * data[10]
        );
    }

    Vector3 transformDirection(const Vector3& vector) const
    {
        return Vector3(
            vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
            vector.x * data[4] + vector.y * data[5] + vector.z * data[6],
            vector.x * data[8] + vector.y * data[9] + vector.z * data[10]
        );
    }

    Vector3 transformInverseDirection(const Vector3& vector) const
    {
        return Vector3(
            vector.x * data[0] + vector.y * data[4] + vector.z * data[8],
            vector.x * data[1] + vector.y * data[5] + vector.z * data[9],
            vector.x * data[2] + vector.y * data[6] + vector.z * data[10]
        );
    }
};

#endif
