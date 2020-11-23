#include "Sphere.h"
#include <Input.h>

Sphere::Sphere(glm::vec2 pos, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, bool isKen) :Rigidbody(SPHERE, pos, velocity, 0, mass, isKen)
{
	m_radius = radius;
	m_colour = colour;
	m_moment = 0.5f * mass * radius * radius;
}

Sphere::~Sphere()
{
}

void Sphere::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::FixedUpdate(gravity, timeStep);
}

void Sphere::MakeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_rotation), std::sin(m_rotation)) * m_radius;
	
	aie::Gizmos::add2DCircle(m_position, m_radius, 24, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1,1,0,1));


}

bool Sphere::CheckCollision(PhysicsObject* pOther)
{
	return false;
}


