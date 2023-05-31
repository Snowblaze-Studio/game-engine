#pragma once

#ifndef PARTICLE_HPP // include guard
#define PARTICLE_HPP

#include "core.hpp"
#include <assert.h>
#include <math.h>

class Particle
{
protected:
    Vector3 position;

    Vector3 velocity;

    Vector3 acceleration;

    float damping;

    float inverseMass;

public:
    void setDamping(const float value)
    {
        damping = value;
    }

    float getDamping() const
    {
        return damping;
    }

    void getAcceleration(Vector3* accPtr) const
    {
        *accPtr = acceleration;
    }

    Vector3 getAcceleration() const
    {
        return acceleration;
    }

    void setAcceleration(const Vector3& value)
    {
        acceleration = value;
    }

    void setAcceleration(const float x, const float y, const float z)
    {
        acceleration.x = x;
        acceleration.y = y;
        acceleration.z = z;
    }

    void setMass(const float value)
    {
        inverseMass = 1.0f / value;
    }

    float getInverseMass() const
    {
        return inverseMass;
    }

    void setInverseMass(const float value)
    {
        inverseMass = value;
    }

    void setPosition(const Vector3& value)
    {
        position = value;
    }

    void setPosition(const float x, const float y, const float z)
    {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    void setVelocity(const Vector3& velPtr)
    {
        velocity = velPtr;
    }

    void setVelocity(const float x, const float y, const float z)
    {
        velocity.x = x;
        velocity.y = y;
        velocity.z = z;
    }

    void getVelocity(Vector3* velPtr) const
    {
        *velPtr = velocity;
    }

    Vector3 getVelocity() const
    {
        return velocity;
    }

    void getPosition(Vector3* positionPtr) const
    {
        *positionPtr = position;
    }

    Vector3 getPosition() const
    {
        return position;
    }

    void integrate(float deltaTime)
    {
        // Skip integrating objects with infinite mass
        if (inverseMass <= 0.0f) return;

        assert(deltaTime > 0.0f);

        // Update position
        position.addScaledVector(velocity, deltaTime);

        Vector3 resultingAcceleration = acceleration;

        // Update velocity
        velocity.addScaledVector(resultingAcceleration, deltaTime);

        // Impose drag
        velocity *= powf(damping, deltaTime);
    }
};

#endif
