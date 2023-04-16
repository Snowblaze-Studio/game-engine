#include "include/groundcontact.hpp"

void GroundContact::init(std::vector<Particle*> particles)
{
    GroundContact::particles = particles;
}

unsigned GroundContact::addContact(ParticleContact* contact, unsigned limit) const
{
    unsigned count = 0;
    for (auto p = particles.begin(); p != particles.end(); p++)
    {
        float y = (*p)->getPosition().y;
        if (y < 0.0f)
        {
            contact->contactNormal = Vector3::UP;
            contact->particle[0] = *p;
            contact->particle[1] = NULL;
            contact->penetration = -y;
            contact->restitution = 0.2f;
            contact++;
            count++;
        }

        if (count >= limit) return count;
    }
    return count;
}