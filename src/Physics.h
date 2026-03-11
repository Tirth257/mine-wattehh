#pragma once
#include "CONSTANTS.h"
#include "back/Vector.h"
class RigidBody
{
private:
public:
    struct AABB
    {
        vec3 min;
        vec3 max;
    } box;
    float mass;
    float velocity;
    vec3 position;

    RigidBody(float mass, float velocity, vec3 position)
    {
        this->mass = mass;
        this->velocity = velocity;
        this->position = position;
    };
    void getCollider(vec3 height, vec3 width, vec3 depth){
        this->box.min = position - (height + width + depth)/2;
        this->box.max = position + (height + width + depth)/2;
    }
};