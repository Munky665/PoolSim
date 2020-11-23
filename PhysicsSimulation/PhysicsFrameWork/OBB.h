#pragma once
#include "Rigidbody.h"


class OBB :
    public Rigidbody
{
public:
    //if setting an object as Kinematic ensure the mass is really, really high.
    OBB(glm::vec2 pos, float x, float y, glm::vec2 vel, float m, glm::vec4 colour, bool isKen = false);
    ~OBB();
    glm::vec2 GetCenter() ;
    bool CheckBoxCorners( OBB& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);
    // Inherited via Rigidbody
    virtual bool CheckCollision(PhysicsObject* pOther);
    void FixedUpdate(glm::vec2 gravity, float timeStep);
    void MakeGizmo();

    glm::vec2 GetSize();
    glm::vec2 GetPosition();
    float GetMass() ;
    float GetWidth() ;
    float GetHeight() ;
   
    glm::vec2 GetLocalY();
    glm::vec2 GetLocalX();

    glm::vec2 GetExtents() ;

private:
    glm::vec2 m_extents;
    glm::vec4 m_colour;

    glm::vec2 m_localX;
    glm::vec2 m_localY;

    glm::vec2 m_size;

};

