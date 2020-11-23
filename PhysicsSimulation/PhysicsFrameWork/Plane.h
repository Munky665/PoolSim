#pragma once
#include "PhysicsObject.h"

class Rigidbody; 
class Plane :
    public PhysicsObject
{
public:
    Plane();
    Plane(glm::vec2 normal, float distance);
    ~Plane();

    // Inherited via PhysicsObject
    virtual void FixedUpdate(glm::vec2 gravity, float timeStep) override;

    virtual void Debug() override;

    virtual void MakeGizmo() override;
    virtual void ResetPosition();

    void ResolveCollision(Rigidbody* object2, glm::vec2 contact);
    
    glm::vec2 GetNormal() { return m_normal; }
    float GetDistance() { return m_distanceToOrigin; }

protected:
    glm::vec2 m_normal;
    float m_distanceToOrigin;
    float m_elasticity;
    // Inherited via PhysicsObject
    virtual float GetKineticEnergy();
};

