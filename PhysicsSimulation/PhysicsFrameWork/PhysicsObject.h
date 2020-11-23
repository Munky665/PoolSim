#pragma once
#include <glm/glm.hpp>
#include <Gizmos.h>

enum ShapeType {
	PLANE = 0,
	SPHERE,
	BOX
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	//virtual void ResetPosition() {};
	virtual float GetKineticEnergy() { return 0; };
	//virtual glm::vec2 GetPosition() = 0;
	//virtual void SetPosition(glm::vec2 pos) = 0;
	//virtual void SetVelocity(glm::vec2 pos) = 0;
	ShapeType GetShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;

};

