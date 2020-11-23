#pragma once
#include "Rigidbody.h"
class Sphere :
    public Rigidbody
{
public:
    //if setting an object as Kinematic ensure the mass is really, really high.
    Sphere(glm::vec2 pos, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, bool isKen = false);
    ~Sphere();

    void FixedUpdate(glm::vec2 gravity, float timeStep);

    virtual void MakeGizmo();
    virtual bool CheckCollision(PhysicsObject* pOther);
    float GetRadius() { return m_radius; }
    glm::vec4 GetColour() { return m_colour; }

protected:
    float m_radius;
    glm::vec4 m_colour;
};

