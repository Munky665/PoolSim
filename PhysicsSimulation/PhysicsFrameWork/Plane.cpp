#include "Plane.h"
#include "Rigidbody.h"
void Plane::FixedUpdate(glm::vec2 gravity, float timeStep) 
{
}

Plane::Plane() : PhysicsObject(PLANE)
{
	m_distanceToOrigin = -40;
	m_normal = glm::vec2(0.0f, 1);
	m_elasticity = 1;
}

Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distance)
{
}

Plane::~Plane()
{
}

void Plane::Debug()
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	
	aie::Gizmos::add2DLine(start, end, colour);
}

void Plane::ResetPosition()
{
}

void Plane::ResolveCollision(Rigidbody* object2, glm::vec2 contact)
{
	// the plane isn't moving, so the relative velocity is just actor2's velocity
	glm::vec2 vRel = object2->GetVelocity();

	float sphereToPlane = glm::dot(object2->GetPosition(), GetNormal()) - GetDistance();

	auto planeNormal = m_normal;

	if (sphereToPlane < 0)
	{
		planeNormal *= -1;
	}

	float e = object2->GetElasticity();

	float j = glm::dot(-(1 + e) * (vRel), planeNormal) / (object2->GetMass());
	
	glm::vec2 force = planeNormal * j;
	
	object2->ApplyForce(force, contact - object2->GetPosition());
}

float Plane::GetKineticEnergy()
{
	return 0.0f;
}
