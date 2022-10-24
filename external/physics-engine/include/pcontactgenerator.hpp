#pragma once

#ifndef PCONTACTGENERATOR_HPP // include guard
#define PCONTACTGENERATOR_HPP

#include "pcontact.hpp"

class ParticleContactGenerator
{
public:
    virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
};

#endif