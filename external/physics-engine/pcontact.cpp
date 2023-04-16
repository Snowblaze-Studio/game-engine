#include "include/pcontact.hpp"

void ParticleContact::resolve(float duration)
{
    resolveVelocity(duration);
    resolveInterpenetration(duration);
}

float ParticleContact::calculateSeparatingVelocity() const
{
    Vector3 relativeVelocity = particle[0]->getVelocity();
    if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
    return relativeVelocity * contactNormal;
}

void ParticleContact::resolveVelocity(float duration)
{
    float separatingVelocity = calculateSeparatingVelocity();

    // The contact is either separating or stationary so no impulse is required.
    if (separatingVelocity > 0)
    {
        return;
    }

    // New separating velocity.
    float newSepVelocity = -separatingVelocity * restitution;

    // Take acceleration buildup into account (also solves resting contacts problem)
    Vector3 accCausedVelocity = particle[0]->getAcceleration();
    if (particle[1]) accCausedVelocity -= particle[1]->getAcceleration();
    float accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

    // If we’ve got a closing velocity due to aceleration buildup, remove it from the new separating velocity.
    if (accCausedSepVelocity < 0)
    {
        newSepVelocity += restitution * accCausedSepVelocity;

        if (newSepVelocity < 0) newSepVelocity = 0;
    }

    float deltaVelocity = newSepVelocity - separatingVelocity;

    float totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

    // Impulses have no effect if both objects have infinite mass
    if (totalInverseMass <= 0) return;

    float impulse = deltaVelocity / totalInverseMass;

    // Impulse per unit of inverse mass.
    Vector3 impulsePerIMass = contactNormal * impulse;

    // Apply impulses
    particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerIMass * particle[0]->getInverseMass());
    if (particle[1])
    {
        // Second object goes in the opposite direction
        particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerIMass * -particle[1]->getInverseMass());
    }
}

void ParticleContact::resolveInterpenetration(float duration)
{
    // If objects don't penetrate, skip this step.
    if (penetration <= 0) return;

    float totalInverseMass = particle[0]->getInverseMass();
    if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

    // If both objects have infinite mass, then we do nothing.
    if (totalInverseMass <= 0) return;

    // Movement per unit of inverse mass.
    Vector3 movePerIMass = contactNormal * (penetration / totalInverseMass);

    // Calculate movement per object.
    particleMovement[0] = movePerIMass * particle[0]->getInverseMass();

    if (particle[1])
    {
        particleMovement[1] = movePerIMass * -particle[1]->getInverseMass();
    }
    else
    {
        particleMovement[1].clear();
    }

    // Apply the movement.
    particle[0]->setPosition(particle[0]->getPosition() + particleMovement[0]);

    if (particle[1])
    {
        particle[1]->setPosition(particle[1]->getPosition() + particleMovement[1]);
    }
}