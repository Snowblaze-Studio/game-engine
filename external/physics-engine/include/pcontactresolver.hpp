#pragma once

#ifndef PCONTACTRESOLVER_HPP // include guard
#define PCONTACTRESOLVER_HPP

#include "pcontact.hpp"

class ParticleContactResolver
{
protected:
    unsigned iterations;

    unsigned iterationsUsed;

public:
    ParticleContactResolver(unsigned iterations);

    void setIterations(unsigned iterations);

    void resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration);
};

#endif