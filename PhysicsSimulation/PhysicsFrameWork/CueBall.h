#pragma once
#include "Sphere.h"
class CueBall :
    public Sphere
{
public:
    CueBall(glm::vec2 pos, glm::vec2 velocity, float mass, float radius, glm::vec4 colour = glm::vec4(1));
    ~CueBall();
    bool IsCue();
    void GetMousePosition(glm::vec2 pos);
private:
    glm::vec2 cueForce = glm::vec2(0);
    glm::vec2 previousPos;
    glm::vec2 currentPos;

    glm::vec2 mousePos;

    bool isCue = false;
};

