#pragma once

#ifndef RIGIDBODY_HPP // include guard
#define RIGIDBODY_HPP

#include "core.hpp"
#include <assert.h>
#include <math.h>

class RigidBody
{
protected:
    Vector3 position;

    Quaternion orientation;

    Vector3 velocity;

    Vector3 acceleration;

    float inverseMass;

    float linearDamping;

    Vector3 angularVelocity;

    Matrix3x4 transformMatrix;

    Matrix3x3 inverseInertiaTensor;

    Matrix3x3 inverseInertiaTensorWorld;

    Vector3 forceAccum;

    Vector3 torqueAccum;

    float angularDamping;

    Vector3 lastFrameAcceleration;

public:
    void setLinearDamping(const float value)
    {
        linearDamping = value;
    }

    float getLinearDamping() const
    {
        return linearDamping;
    }

    void setAngularDamping(const float value)
    {
        angularDamping = value;
    }

    float getAngularDamping() const
    {
        return angularDamping;
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
        // Calculate linear acceleration from force inputs
        lastFrameAcceleration = acceleration;
        lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

        // Calculate angular acceleration from torque inputs
        Vector3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);

        // Adjust velocities
        // Update linear velocity from both acceleration and impulse
        velocity.addScaledVector(lastFrameAcceleration, deltaTime);

        // Update angular velocity from both acceleration and impulse
        angularVelocity.addScaledVector(angularAcceleration, deltaTime);

        // Impose drag
        velocity *= powf(linearDamping, deltaTime);
        angularVelocity *= powf(angularDamping, deltaTime);

        // Adjust positions
        // Update linear position
        position.addScaledVector(velocity, deltaTime);

        // Update angular position
        orientation.addScaledVector(angularVelocity, deltaTime);

        // Normalise the orientation, and update the matrices with the new
        // position and orientation
        calculateDerivedData();

        // Clear accumulators
        clearAccumulators();
    }

    void setOrientation(const Quaternion& value)
    {
        orientation = value;
        orientation.normalize();
    }

    void setOrientation(const float x, const float y, const float z, const float w)
    {
        orientation.x = x;
        orientation.y = y;
        orientation.z = z;
        orientation.w = w;
        orientation.normalize();
    }

    void getOrientation(Quaternion* orientationPtr) const
    {
        *orientationPtr = orientation;
    }

    Quaternion getOrientation() const
    {
        return orientation;
    }

    void calculateDerivedData()
    {
        orientation.normalize();

        _calculateTransformMatrix(transformMatrix, position, orientation);

        _transformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
    }

    void setInertiaTensor(const Matrix3x3& value)
    {
        inverseInertiaTensor.setInverse(value);
    }

    void getInertiaTensor(Matrix3x3* inertiaTensorPtr) const
    {
        inertiaTensorPtr->setInverse(inverseInertiaTensor);
    }

    Matrix3x3 getInertiaTensor() const
    {
        Matrix3x3 inertiaTensor;
        getInertiaTensor(&inertiaTensor);
        return inertiaTensor;
    }

    void setInverseInertiaTensor(const Matrix3x3& value)
    {
        inverseInertiaTensor = value;
    }

    void getInverseInertiaTensor(Matrix3x3* inverseInertiaTensorPtr) const
    {
        *inverseInertiaTensorPtr = inverseInertiaTensor;
    }

    Matrix3x3 getInverseInertiaTensor() const
    {
        return inverseInertiaTensor;
    }

    void getInertiaTensorWorld(Matrix3x3* inertiaTensorPtr) const
    {
        inertiaTensorPtr->setInverse(inverseInertiaTensorWorld);
    }

    Matrix3x3 getInertiaTensorWorld() const
    {
        Matrix3x3 it;
        getInertiaTensorWorld(&it);
        return it;
    }

    void getInverseInertiaTensorWorld(Matrix3x3* inverseInertiaTensorPtr) const
    {
        *inverseInertiaTensorPtr = inverseInertiaTensorWorld;
    }

    Matrix3x3 getInverseInertiaTensorWorld() const
    {
        return inverseInertiaTensorWorld;
    }

    void addForce(const Vector3& force)
    {
        forceAccum += force;
    }

    void addTorque(const Vector3& torque)
    {
        torqueAccum += torque;
    }

    void clearAccumulators()
    {
        forceAccum.clear();
        torqueAccum.clear();
    }

    void addForceAtPoint(const Vector3& force, const Vector3& point)
    {
        Vector3 pt = point;
        pt -= position;

        forceAccum += force;
        torqueAccum += pt % force;
    }

    void addForceAtBodyPoint(const Vector3& force, const Vector3& point)
    {
        Vector3 pt = getPointInWorldSpace(point);
        addForceAtPoint(force, pt);
    }

    Vector3 getPointInWorldSpace(const Vector3& point) const
    {
        return transformMatrix.transform(point);
    }

    void getLastFrameAcceleration(Vector3* accelerationPtr) const
    {
        *accelerationPtr = lastFrameAcceleration;
    }

    Vector3 getLastFrameAcceleration() const
    {
        return lastFrameAcceleration;
    }

private:
    void _calculateTransformMatrix(Matrix3x4& transformMatrix, const Vector3& position, const Quaternion& orientation)
    {
        transformMatrix.data[0] = 1.0f - 2.0f * orientation.y * orientation.y - 2.0f * orientation.z * orientation.z;
        transformMatrix.data[1] = 2.0f * orientation.x * orientation.y + 2.0f * orientation.z * orientation.w;
        transformMatrix.data[2] = 2.0f * orientation.x * orientation.z - 2.0f * orientation.y * orientation.w;
        transformMatrix.data[3] = position.x;

        transformMatrix.data[4] = 2.0f * orientation.x * orientation.y - 2.0f * orientation.z * orientation.w;
        transformMatrix.data[5] = 1.0f - 2.0f * orientation.x * orientation.x - 2.0f * orientation.z * orientation.z;
        transformMatrix.data[6] = 2.0f * orientation.y * orientation.z + 2.0f * orientation.x * orientation.w;
        transformMatrix.data[7] = position.y;

        transformMatrix.data[8] = 2.0f * orientation.x * orientation.z + 2.0f * orientation.y * orientation.w;
        transformMatrix.data[9] = 2.0f * orientation.y * orientation.z - 2.0f * orientation.x * orientation.w;
        transformMatrix.data[10] = 1.0f - 2.0f * orientation.x * orientation.x - 2.0f * orientation.y * orientation.y;
        transformMatrix.data[11] = position.z;
    }

    void _transformInertiaTensor(Matrix3x3& iitWorld, const Quaternion& q, const Matrix3x3& iitBody, const Matrix3x4& rotmat)
    {
        float t4 = rotmat.data[0] * iitBody.data[0] + rotmat.data[1] * iitBody.data[3] + rotmat.data[2] * iitBody.data[6];
        float t9 = rotmat.data[0] * iitBody.data[1] + rotmat.data[1] * iitBody.data[4] + rotmat.data[2] * iitBody.data[7];
        float t14 = rotmat.data[0] * iitBody.data[2] + rotmat.data[1] * iitBody.data[5] + rotmat.data[2] * iitBody.data[8];
        float t28 = rotmat.data[4] * iitBody.data[0] + rotmat.data[5] * iitBody.data[3] + rotmat.data[6] * iitBody.data[6];
        float t33 = rotmat.data[4] * iitBody.data[1] + rotmat.data[5] * iitBody.data[4] + rotmat.data[6] * iitBody.data[7];
        float t38 = rotmat.data[4] * iitBody.data[2] + rotmat.data[5] * iitBody.data[5] + rotmat.data[6] * iitBody.data[8];
        float t52 = rotmat.data[8] * iitBody.data[0] + rotmat.data[9] * iitBody.data[3] + rotmat.data[10] * iitBody.data[6];
        float t57 = rotmat.data[8] * iitBody.data[1] + rotmat.data[9] * iitBody.data[4] + rotmat.data[10] * iitBody.data[7];
        float t62 = rotmat.data[8] * iitBody.data[2] + rotmat.data[9] * iitBody.data[5] + rotmat.data[10] * iitBody.data[8];

        iitWorld.data[0] = t4 * rotmat.data[0] + t9 * rotmat.data[1] + t14 * rotmat.data[2];
        iitWorld.data[1] = t4 * rotmat.data[4] + t9 * rotmat.data[5] + t14 * rotmat.data[6];
        iitWorld.data[2] = t4 * rotmat.data[8] + t9 * rotmat.data[9] + t14 * rotmat.data[10];
        iitWorld.data[3] = t28 * rotmat.data[0] + t33 * rotmat.data[1] + t38 * rotmat.data[2];
        iitWorld.data[4] = t28 * rotmat.data[4] + t33 * rotmat.data[5] + t38 * rotmat.data[6];
        iitWorld.data[5] = t28 * rotmat.data[8] + t33 * rotmat.data[9] + t38 * rotmat.data[10];
        iitWorld.data[6] = t52 * rotmat.data[0] + t57 * rotmat.data[1] + t62 * rotmat.data[2];
        iitWorld.data[7] = t52 * rotmat.data[4] + t57 * rotmat.data[5] + t62 * rotmat.data[6];
        iitWorld.data[8] = t52 * rotmat.data[8] + t57 * rotmat.data[9] + t62 * rotmat.data[10];
    }
};

#endif