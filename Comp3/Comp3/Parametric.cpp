#include "Parametric.h"



float Parametric::x(float t) {
    return std::cos(t);
}

float Parametric::y(float t) {
    return std::sin(t);
}

float Parametric::z(float t) {
    return t;
}

glm::vec3 Parametric::mapPointToSurface(float t, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 p(x(t), y(t), z(t));
    float surface_y = character.barycentricCoordinates(p, a, b, c);
    float offset = 0.4f;
    return glm::vec3(x(t), surface_y + offset, z(t));
}








