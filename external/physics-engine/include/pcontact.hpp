#pragma once

#ifndef PCONTACT_HPP // include guard
#define PCONTACT_HPP

#include "particle.hpp"

class ParticleContact
{
    friend class ParticleContactResolver;

public:
    Particle* particle[2];

    float restitution;

    Vector2 contactNormal;

    float penetration;

    Vector2 particleMovement[2];

protected:
    void resolve(float duration);

    float calculateSeparatingVelocity() const;

private:
    void resolveVelocity(float duration);

    void resolveInterpenetration(float duration);
};

#endif