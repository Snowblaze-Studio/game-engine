#pragma once

#include "rigidbody.hpp"
#include "bounding_sphere.hpp"

#ifndef BOUNDING_VOLUME_HIERARCHY_HPP
#define BOUNDING_VOLUME_HIERARCHY_HPP

struct PotentialContact
{
    RigidBody* body[2];
};

class BVHNode
{
public:
    BVHNode* children[2];

    BoundingSphere volume;

    RigidBody* body;

    BVHNode* parent;

    BVHNode(BVHNode* parent, const BoundingSphere& volume, RigidBody* body = nullptr) : parent(parent), volume(volume), body(body)
    {
        children[0] = children[1] = nullptr;
    }

    bool isLeaf() const
    {
        return body != nullptr;
    }

    unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const
    {
        if (isLeaf() || limit == 0) return 0;

        return children[0]->getPotentialContactsWith(children[1], contacts, limit);
    }

    void insert(RigidBody* newBody, const BoundingSphere& newVolume)
    {
        if (isLeaf())
        {
            children[0] = new BVHNode(this, volume, body);
            children[1] = new BVHNode(this, newVolume, newBody);

            this->body = nullptr;

            recalculateBoundingVolume();
        }
        else
        {
            if (children[0]->volume.getGrowth(newVolume) < children[1]->volume.getGrowth(newVolume))
            {
                children[0]->insert(newBody, newVolume);
            }
            else
            {
                children[1]->insert(newBody, newVolume);
            }
        }
    }

    ~BVHNode()
    {
        if (parent)
        {
            BVHNode* sibling;
            if (parent->children[0] == this) sibling = parent->children[1];
            else sibling = parent->children[0];

            parent->volume = sibling->volume;
            parent->body = sibling->body;
            parent->children[0] = sibling->children[0];
            parent->children[1] = sibling->children[1];

            sibling->parent = nullptr;
            sibling->body = nullptr;
            sibling->children[0] = nullptr;
            sibling->children[1] = nullptr;
            delete sibling;

            parent->recalculateBoundingVolume();
        }

        if (children[0]) {
            children[0]->parent = nullptr;
            delete children[0];
        }
        if (children[1]) {
            children[1]->parent = nullptr;
            delete children[1];
        }
    }

protected:
    bool overlaps(const BVH* other) const
    {
        return volume->overlaps(other->volume);
    }

    unsigned getPotentialContactsWith(const BVHNode* other, PotentialContact* contacts, unsigned limit) const
    {
        if (!overlaps(other) || limit == 0) return 0;

        if (isLeaf() && other->isLeaf())
        {
            contacts->body[0] = body;
            contacts->body[1] = other->body;
            return 1;
        }

        if (other->isLeaf() || (!isLeaf() && volume->getSize() >= other->volume->getSize()))
        {
            unsigned count = children[0]->getPotentialContactsWith(other, contacts, limit);

            if (limit > count)
            {
                return count + children[1]->getPotentialContactsWith(other, contacts + count, limit - count);
            }
            else
            {
                return count;
            }
        }
        else
        {
            unsigned count = getPotentialContactsWith(other->children[0], contacts, limit);

            if (limit > count)
            {
                return count + getPotentialContactsWith(other->children[1], contacts + count, limit - count);
            }
            else
            {
                return count;
            }
        }
    }

    void recalculateBoundingVolume()
    {
        if (isLeaf()) return;

        volume = BoundingSphere(children[0]->volume, children[1]->volume);

        if (parent) parent->recalculateBoundingVolume();
    }
};

#endif