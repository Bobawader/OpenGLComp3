#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"


class Character {
public:
    glm::vec3 position;
    float speed;

    Character(glm::vec3 startPosition, float startSpeed);

    void move(GLFWwindow* window, std::vector<std::vector<Vertex>>& surfaceVertices, int width, int height);
    float barycentricCoordinates(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
};


