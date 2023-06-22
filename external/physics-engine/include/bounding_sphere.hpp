#pragma once

#include <math.h>
#include "core.hpp"

#ifndef BOUNDING_SPHERE_HPP
#define BOUNDING_SPHERE_HPP

struct BoundingSphere
{
    Vector3 center;
    float radius;

public:
    BoundingSphere(const Vector3& center, float radius)
    {
        this.center = center;
        this.radius = radius;
    }

    BoundingSphere(const BoundingSphere& a, const BoundingSphere& b)
    {
        Vector3 centerOffset = b.center - a.center;
        // Squared distance between centers
        float distance = centerOffset.x * centerOffset.x + centerOffset.y * centerOffset.y + centerOffset.z * centerOffset.z;
        float radiusDiff = b.radius - a.radius;

        if (radiusDiff * radiusDiff >= distance)
        {
            if (a.radius > b.radius)
            {
                center = a.center;
                radius = a.radius;
            }
            else
            {
                center = b.center;
                radius = b.radius;
            }
        }
        else
        {
            distance = sqrt(distance);
            radius = (distance + a.radius + b.radius) * 0.5f;

            center = a.center;
            if (distance > 0)
            {
                center += centerOffset * ((radius - a.radius) / distance);
            }
        }
    }

    bool overlaps(const BoundingSphere* other) const
    {
        Vector3 centerOffset = center - other->center;
        float distanceSquared = centerOffset.x * centerOffset.x + centerOffset.y * centerOffset.y + centerOffset.z * centerOffset.z;

        return distanceSquared < (radius + other->radius) * (radius + other->radius);
    }

    float getGrowth(const BoundingSphere& other) const
    {
        BoundingSphere newSphere(*this, other);

        return newSphere.radius * newSphere.radius - radius * radius;
    }
};

#endif