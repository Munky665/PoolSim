#include "CueBall.h"
#include <Input.h>


CueBall::CueBall(glm::vec2 pos, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) : Sphere(pos, velocity,mass,radius,colour)
{
	isCue = true;
}

CueBall::~CueBall()
{
}

bool CueBall::IsCue()
{
	if (this != nullptr)
	{
		return isCue;
	}
	else
		return false;
}


void CueBall::GetMousePosition(glm::vec2 pos)
{
	mousePos = pos;
		aie::Input* input = aie::Input::getInstance();
		glm::vec2 clamp(150, 150);

	if (input->isMouseButtonUp(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		if (cueForce != glm::vec2(0))
		{
			if (cueForce.x > clamp.x)
			{
				if (cueForce.y > clamp.y)
				{
					cueForce = clamp;
				}
				else
				{
					cueForce.x = clamp.x;
				}
			}
			else if (cueForce.y > clamp.y)
			{
				cueForce.y = clamp.y;
			}
			
			if (cueForce.x < -clamp.x)
			{
				if (cueForce.y < -clamp.y)
				{
					cueForce = -clamp;
				}
				else
				{
					cueForce.x = -clamp.x;
				}
			}
			else if (cueForce.y < -clamp.y)
			{
				cueForce.y = -clamp.y;
			}

			ApplyForce(glm::vec2(-cueForce.x, cueForce.y), m_position);
			cueForce = glm::vec2(0);
		}
		previousPos = mousePos;
	}
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		currentPos = mousePos;
		
		cueForce = currentPos - previousPos;

	}
}
