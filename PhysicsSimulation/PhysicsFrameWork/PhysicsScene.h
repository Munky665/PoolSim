#pragma once
#include "PhysicsObject.h"
#include <vector>

class Rigidbody;

class PhysicsScene
{
public:

	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);
	void processPendingRemovals();
	void Update(float dt);
	void UpdateGizmos();
	int GetScore();
	void CheckForCollision();
	
	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 GetGravity() const { return m_gravity; }
	
	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }
	
	static void ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen);
	static bool Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plan2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plan2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Box(PhysicsObject* obj1, PhysicsObject* obj2);
protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	int m_score = 0;
	std::vector<PhysicsObject*> m_actors;
	std::vector<PhysicsObject*> m_pendingRemovals;
	std::vector<PhysicsObject*> m_pending_adds;

};

