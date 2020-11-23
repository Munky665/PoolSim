#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include "OBB.h"
#include "CueBall.h"
#include <iostream>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
#define SHAPE_COUNT 3

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plan2Plane,	PhysicsScene::Plane2Sphere, PhysicsScene::Plan2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere,PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane,	PhysicsScene::Box2Sphere,	PhysicsScene::Box2Box
};

void PhysicsScene::CheckForCollision()
{
	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->GetShapeID();
			int shapeId2 = object2->GetShapeID();

			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn CollisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (CollisionFunctionPtr != nullptr)
			{
				float kePre = object1->GetKineticEnergy() + object2->GetKineticEnergy();
				
				//did a collision occur?
				if (CollisionFunctionPtr(object1, object2))
				{
					std::cout << "Collision Processed" << object1->GetShapeID() << ", " << object2->GetShapeID() << std::endl;
				}
				float kePost = object1->GetKineticEnergy() + object2->GetKineticEnergy();

				float deltaKe = kePost - kePre;
				if (deltaKe < -0.01f || deltaKe > 0.01f)
				{
					std::cout << "Energy change detected!" << std::endl;
				}
			}
		}
	}
}

void PhysicsScene::ApplyContactForces(Rigidbody* b1, Rigidbody* b2, glm::vec2 norm, float pen)
{
	float b1Factor = b1->IsKinematic() ? 0 : (b2->IsKinematic() ? 1.0f : 0.5f);
	if (b1->IsKinematic() && b2->IsKinematic())
	{
		//allows Kinematic objects to overlap without spasming
	}
	else
	{
		b1->SetPosition(b1->GetPosition() - b1Factor * norm * pen);
		b2->SetPosition(b2->GetPosition() + (1 - b1Factor) * norm * pen);
	}
}

PhysicsScene::PhysicsScene() : m_timeStep(0.001f), m_gravity(glm::vec2(0,0))
{

}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_pending_adds.push_back(actor);
}


void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	m_pendingRemovals.push_back(actor);

}

void PhysicsScene::processPendingRemovals()
{
	std::vector<PhysicsObject*> tempv;
	tempv.reserve(m_actors.size());
	if (m_pendingRemovals.size() != 0) {
		for (int i = 0, j = 0; i < m_actors.size(); ++i)
		{
			if (m_actors[i] != m_pendingRemovals[j])
			{
				tempv.push_back(m_actors[i]);
			}
			else
			{
				j++;
			}
		}

		m_actors = std::move(tempv);

		m_pendingRemovals.clear();
	}
}

void PhysicsScene::Update(float dt)
{
	static std::vector<PhysicsObject*> dirty;

	//update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	m_pending_adds.reserve(1);

	while (accumulatedTime >= m_timeStep)
	{
		for (auto po : m_actors)
		{
			po->FixedUpdate(m_gravity, m_timeStep);
			if (abs(dynamic_cast<Rigidbody*>(po)->GetPosition().x) > 62 || abs(dynamic_cast<Rigidbody*>(po)->GetPosition().y) > 28)
			{
				if (dynamic_cast<CueBall*>(po)->IsCue())
				{
					dynamic_cast<Rigidbody*>(po)->SetPosition(glm::vec2(-40, 0));
					dynamic_cast<Rigidbody*>(po)->SetVelocity(glm::vec2(0));
					m_score -= 5;
				}
				else
				{
					if (abs(dynamic_cast<Rigidbody*>(po)->GetPosition().x) < 70 || abs(dynamic_cast<Rigidbody*>(po)->GetPosition().y) < 30)
					{
						m_score += 5;
					}
					//move object well off screen
					dynamic_cast<Rigidbody*>(po)->SetPosition(glm::vec2(1000));

				}
			}
		}
		accumulatedTime -= m_timeStep;
	}



	//insert pending adds into the main list
	m_actors.reserve(m_actors.size() + m_pending_adds.size());
	m_actors.insert(m_actors.end(), m_pending_adds.begin(), m_pending_adds.end());
	m_pending_adds.clear();

	CheckForCollision();

}

void PhysicsScene::UpdateGizmos()
{
	aie::Gizmos::add2DCircle(glm::vec2(0, 25), 5, 24, glm::vec4(0, 0, 0, 1));
	aie::Gizmos::add2DCircle(glm::vec2(0, -25), 5, 24, glm::vec4(0, 0, 0, 1));
	aie::Gizmos::add2DCircle(glm::vec2(60, 25), 5, 24, glm::vec4(0, 0, 0, 1));
	aie::Gizmos::add2DCircle(glm::vec2(-60, 25), 5, 24, glm::vec4(0, 0, 0, 1));
	aie::Gizmos::add2DCircle(glm::vec2(60, -25), 5, 24, glm::vec4(0, 0, 0, 1));
	aie::Gizmos::add2DCircle(glm::vec2(-60, -25), 5, 24, glm::vec4(0, 0, 0, 1));

	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}


}

int PhysicsScene::GetScore()
{
	return m_score;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cas objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);


	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{

		glm::vec2 delta = sphere2->GetPosition() - sphere1->GetPosition();
		float distance = glm::length(delta);
		float intersection = sphere1->GetRadius() + sphere2->GetRadius() - distance;

		if (intersection > 0)
		{
			glm::vec2 contactForce = 0.5f * (distance - (sphere1->GetRadius() + sphere2->GetRadius())) * delta / distance;

			sphere1->SetPosition(sphere1->GetPosition() + contactForce);
			sphere2->SetPosition(sphere2->GetPosition() - contactForce);

			sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->GetPosition() + sphere2->GetPosition())/* / 2.0f*/);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	
	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float sphereToPlane = glm::dot(sphere->GetPosition(), plane->GetNormal()) - plane->GetDistance();
		
		//planes are infinitely thin, double sided objects so if we are behind it we flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->GetRadius() - sphereToPlane;

		if (intersection > 0)
		{
	

			glm::vec2 contact = sphere->GetPosition() + (collisionNormal * -sphere->GetRadius());

			plane->ResolveCollision(sphere, contact);

			sphere->SetPosition(sphere->GetPosition() + plane->GetNormal() * (sphere->GetRadius() - sphereToPlane));

			return true;
		}
	}

	return false;
}

bool PhysicsScene::Plan2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Plan2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	OBB* box = dynamic_cast<OBB*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if(box != nullptr&& plane != nullptr)
	{
		int numContacts = 0;glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->GetWidth(), box->GetHeight());
		float penetration = 0;
		
		// which side is the centre of mass on?
		glm::vec2 planeOrigin = plane->GetNormal() *plane->GetDistance();
		float comFromPlane = glm::dot(box->GetPosition() -planeOrigin, plane->GetNormal());
		// check all four corners to see if we've hit the plane
		for(float x = -box->GetExtents().x; x<box->GetWidth(); x += box->GetWidth())
		{
			for(float y=-box->GetExtents().y; y<box->GetHeight(); y+=box->GetHeight())
			{
				// get the position of the corner in world space
				glm::vec2 p = box->GetPosition() +x *box->GetLocalX() +y *box->GetLocalY();
				float distFromPlane = glm::dot(p -planeOrigin, plane->GetNormal());
				// this is the total velocity of the point
				float velocityIntoPlane = glm::dot(box->GetVelocity()+box->GetAngularVelocity() *(-y *box->GetLocalX() +x *box->GetLocalY()), plane->GetNormal());
				// if this corner is on the opposite side from the COM, 
				// and moving further in, we need to resolve the collision
				if((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane >= 0)
					||(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane <= 0))
				{
					numContacts++;contact +=p;contactV += velocityIntoPlane;
				}
				if (comFromPlane >= 0)
				{ 
					if (penetration > distFromPlane)
						penetration = distFromPlane; 
				}
				else 
				{ 
					if (penetration < distFromPlane)
						penetration = distFromPlane; 
				}
			}
		}
		//we've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			//get the average collision velocity into the plane
			//(covers linear and rotation velocity of all corners involved
			float collisionV = contactV / (float)numContacts;
			//get the acceleration required to stop (restitution = 0) or reverse
			//(restitution = 1) the average velocity into the plane
			glm::vec2 acceleration = -plane->GetNormal() * ((1.0f + box->GetElasticity()) * collisionV);
			//and the average position at which we'll apply the force
			//(corner or edge centre)
			glm::vec2 localContact = (contact / (float)numContacts) - box->GetPosition();
			//this is the perpendicular distance we apply the force at relative to
			//the COM, so torque = F*r
			float r = glm::dot(localContact, glm::vec2(plane->GetNormal().y, -plane->GetNormal().x));
			//work out the "effective mass" - this is a combination of moment of
			//inertia and mass, and tells us how muc th contact point velocity
			//will change the force we're applying
			float mass0 = 1.0f / (1.0f / box->GetMass() + (r * r) / box->GetMoment());

			//apply the force
			box->ApplyForce(acceleration * mass0, localContact);

			box->SetPosition(box->GetPosition() - plane->GetNormal() * penetration);

			return true;
		}
	}
	return false;
}


bool PhysicsScene::Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//try to cas objects to sphere and sphere
	OBB* box = dynamic_cast<OBB*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);

	//if we are successful then test for collision
	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 circlePos = sphere->GetPosition() - box->GetPosition();
		float w2 = box->GetWidth() / 2, h2 = box->GetHeight() / 2;

		int numContacts = 0;
		glm::vec2 contact(0,0); //contact is in our box coordinates

		//check the four corners to see if any of there are inside the circle
		for (float x = -w2; x <= w2; x += box->GetWidth())
		{
			for (float y = -h2; y <= h2; y += box->GetHeight())
			{
				glm::vec2 p = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x * dp.x + dp.y * dp.y < sphere->GetRadius() * sphere->GetRadius())
				{
					numContacts++;
					contact += glm::vec2(x, y);
				}
			}
		}
		glm::vec2* direction = nullptr;
		//get the local position of the circle centre
		glm::vec2 localPos(glm::dot(box->GetLocalX(), circlePos), 
						   glm::dot(box->GetLocalY(), circlePos));
		if (localPos.y < h2 && localPos.y > -h2)
		{
			if (localPos.x > 0 && localPos.x < w2 + sphere->GetRadius())
			{
				numContacts++;
				contact += glm::vec2(w2, localPos.y);
				direction = new glm::vec2(box->GetLocalX());
			}
			if (localPos.x < 0 && localPos.x > -(w2 + sphere->GetRadius()))
			{
				numContacts++; 
				contact += glm::vec2(-w2, localPos.y);
				direction = new glm::vec2(-box->GetLocalX());
			}
		}
		if (localPos.x < w2 && localPos.x > -w2)
		{ 
			if (localPos.y > 0 && localPos.y < h2 + sphere->GetRadius()) 
			{ 
				numContacts++; 
				contact += glm::vec2(localPos.x, h2);
				direction = new glm::vec2(box->GetLocalY());
			}
			if (localPos.y < 0 && localPos.y > -(h2 + sphere->GetRadius()))
			{ 
				numContacts++; 
				contact += glm::vec2(localPos.x, -h2);
				direction = new glm::vec2(-box->GetLocalY());
			} 
		}

		if (numContacts > 0)
		{
			contact = box->GetPosition() + (1.0f / numContacts) * (box->GetLocalX() * contact.x + box->GetLocalY() * contact.y);
			
			box->ResolveCollision(sphere, contact, direction);

			//given the contact point we can find a penetration amount and normal
			float pen = sphere->GetRadius() - glm::length(contact - sphere->GetPosition());

			glm::vec2 norm = glm::normalize(sphere->GetPosition() - contact);

			ApplyContactForces(box, sphere, norm, pen);
		}
	
		delete direction;
	
	}
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	OBB* box1 = dynamic_cast<OBB*>(obj1);
	OBB* box2 = dynamic_cast<OBB*>(obj2);

	if (box1 != nullptr && box2 != nullptr)
	{
		glm::vec2 boxPos = box2->GetCenter() - box1->GetCenter();

		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;

		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);

		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm))
		{
			norm = -norm;
		}
		if (pen > 0)
		{
			box1->ResolveCollision(box2, contact / float(numContacts), &norm);

			ApplyContactForces(box1, box2, norm, pen);

			return true;
		}
	}
	 return false;
}
