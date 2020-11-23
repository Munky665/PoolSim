#include "Rigidbody.h"
#include "OBB.h"
#define MIN_LINEAR_THRESHOLD 0.25f
#define MIN_ROTATIONAL_THRESHOLD 0.25f

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 pos, glm::vec2 velocity, float rotation, float mass, bool isKen) : PhysicsObject(shapeID), m_position(pos),m_velocity(velocity), m_rotation(rotation), m_mass(mass), m_isKenimatic(isKen)
{
	m_angularVelocity = 0.01f;
	m_linearDrag = 0.3;
	m_angularDrag = 0.1f;
	m_elasticity = 1;
	m_moment = 1;

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKenimatic)
	{
		return;
	}
	//remember when applying the force of gravity, mass cancels out
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
	

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ROTATIONAL_THRESHOLD)
	{
		m_angularVelocity = 0;
	}
}

void Rigidbody::Debug()
{
}

void Rigidbody::MakeGizmo()
{
}

void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);

	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->GetPosition(), perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;


	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->GetElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;


		ApplyForce(-force, contact - m_position);

		actor2->ApplyForce(force, contact - actor2->GetPosition());
	}

}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);

}

void Rigidbody::SetPosition(glm::vec2 pos)
{
	m_position = pos;
}

void Rigidbody::SetVelocity(glm::vec2 vel)
{
	m_velocity = vel;
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

