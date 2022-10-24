#pragma once

#ifndef GROUNDCONTACT_HPP // include guard
#define GROUNDCONTACT_HPP

#include <vector>
#include "particle.hpp"
#include "pcontact.hpp"
#include "pcontactgenerator.hpp"

class GroundContact : public ParticleContactGenerator
{
public:
    void init(std::vector<Particle*> particles);

    virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;

private:
    std::vector<Particle*> particles;
};

#endif