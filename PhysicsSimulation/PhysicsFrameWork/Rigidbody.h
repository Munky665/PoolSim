#pragma once
#include "PhysicsObject.h"
class Rigidbody :
    public PhysicsObject
{
public:
    //if setting an object as Kinematic ensure the mass is really, really high.
    Rigidbody(ShapeType shapeID, glm::vec2 pos, glm::vec2 velocity, float rotation, float mass, bool isKen);
    ~Rigidbody();

    // Inherited via PhysicsObject
    virtual void FixedUpdate(glm::vec2 gravity, float timeStep) override;
    virtual void Debug() override;
    virtual void MakeGizmo() override;
    
    virtual bool CheckCollision(PhysicsObject* pOther) = 0;

    void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);

    void ApplyForce(glm::vec2 force, glm::vec2 pos);
    //void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);
    void SetPosition(glm::vec2 pos);
    void SetVelocity(glm::vec2 vel);
    glm::vec2 GetPosition() { return m_position; }
    float GetRotation() { return m_rotation; }
    glm::vec2 GetVelocity() { return m_velocity; }
    float GetMass() { return m_mass; }
    float GetElasticity() { return m_elasticity; }
    float GetAngularVelocity() { return m_angularVelocity; }
    float GetMoment() { return m_moment; }
    float GetKineticEnergy();
    bool IsKinematic() { return m_isKenimatic; }
protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_rotation;
    float m_linearDrag = 0.3f;
    float m_angularDrag = 0.3f;
    float m_angularVelocity;
    float m_moment;
    float m_elasticity;
    bool m_isKenimatic;
};

