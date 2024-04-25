#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include "character.h"

class Parametric {
public:
    
    /*Parametric() : character(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f) {

    }*/

    Parametric();

     float x(float t);
     float y(float t);
     float z(float t);

     glm::vec3 mapPointToSurface(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c);

private:
    Character character;
};





