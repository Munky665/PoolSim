#include "OBB.h"
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm\gtx\rotate_vector.hpp>



OBB::OBB(glm::vec2 pos, float x, float y, glm::vec2 vel, float m, glm::vec4 colour, bool isKen) : Rigidbody(BOX, pos, vel, 0, m, isKen)
{
    m_extents.x = x / 2;
    m_extents.y = y /2;
    m_size.x = x;
    m_size.y = y;
    m_colour = colour;
    m_moment = 1.0f / 12.0f * m * x * y;
}

OBB::~OBB()
{
}

glm::vec2 OBB::GetCenter()
{
    return glm::vec2(m_position.x + (m_extents.x / 2), m_position.y + (m_extents.y / 2));
}

bool OBB::CheckBoxCorners(OBB& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal)
{
    float minX, maxX, minY, maxY;
    float boxW = box.GetExtents().x * 2; 
    float boxH = box.GetExtents().y * 2;
    int numLocalContacts = 0;
    glm::vec2 localContact(0, 0);

    bool first = true;

    for (float x = -box.GetExtents().x; x < boxW; x += boxW)
    {
        for (float y = -box.GetExtents().y; y < boxH; y += boxH)
        {
            // position in worldspace
            glm::vec2 p = box.GetCenter() + x * box.m_localX + y * box.m_localY;
            // position in our box's space
            glm::vec2 p0(glm::dot(p - m_position, m_localX),
                         glm::dot(p - m_position, m_localY));

            if (first || p0.x < minX)
                minX = p0.x;
            if (first || p0.x > maxX)
                maxX = p0.x;
            if (first || p0.y < minY)
                minY = p0.y;
            if (first || p0.y > maxY)
                maxY = p0.y;

            if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
                p0.y >= -m_extents.y && p0.y <= m_extents.y)
            {
                numLocalContacts++; 
                localContact += p0;
            }
            first = false;
        }
    }
    if (maxX < -m_extents.x || minX > m_extents.x || 
        maxY< -m_extents.y || minY > m_extents.y)
        return false; 
    if (numLocalContacts == 0)
        return false;
    
    bool res = false;

    contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) / (float)numLocalContacts;
    numContacts++;

    float pen0 = m_extents.x - minX; 

    if (pen0 > 0 && (pen0 < pen || pen == 0)) 
    { 
        edgeNormal = m_localX; 
        pen = pen0;
        res = true; 
    }
    pen0 = maxX + m_extents.x;
    if (pen0 > 0 && (pen0 < pen || pen == 0)) 
    { 
        edgeNormal = -m_localX;
        pen = pen0;
        res = true; 
    }
    pen0 = m_extents.y - minY; 
    if (pen0 > 0 && (pen0 < pen || pen == 0)) 
    {
        edgeNormal = m_localY;
        pen = pen0; 
        res = true; 
    }
    pen0 = maxY + m_extents.y; 
    if (pen0 > 0 && (pen0 < pen || pen == 0))
    {
        edgeNormal = -m_localY; 
        pen = pen0; 
        res = true; 
    }
    return res;
}
bool OBB::CheckCollision(PhysicsObject* pOther)
{
    return false;
}

void OBB::FixedUpdate(glm::vec2 gravity, float timeStep)
{
    Rigidbody::FixedUpdate(gravity, timeStep);

    //store local axes
    float cs = std::cosf(m_rotation);
    float sn = std::sinf(m_rotation);

    m_localX = glm::normalize(glm::vec2(cs, sn));
    m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void OBB::MakeGizmo()
{
    /*aie::Gizmos::add2DAABBFilled(m_position, glm::vec2(m_size[0], m_size[1]), m_colour);*/
    
    //glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0, 0, 1));
    //aie::Gizmos::add2DAABBFilled(GetCenter(), m_extents, m_colour, &transform);

    //draw using local axes
    glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
    glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y; 
    glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y; 
    glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
    aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
    aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

glm::vec2 OBB::GetSize()
{
    return m_size;
}

glm::vec2 OBB::GetPosition()
{
    return m_position;
}

float OBB::GetMass()
{
    return m_mass;
}

float OBB::GetWidth()  
{
    return m_size.x;
}

float OBB::GetHeight() 
{
    return m_size.y;
}

glm::vec2 OBB::GetExtents() 
{
    return m_extents;
}

glm::vec2 OBB::GetLocalY()
{
    return m_localY;
}

glm::vec2 OBB::GetLocalX()
{
    return m_localX;
}

