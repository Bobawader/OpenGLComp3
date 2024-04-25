#pragma once
#include <glm/vec3.hpp>


struct GLFWwindow;


class Character {
public:
    glm::vec3 position;
    float speed;
    float* heightmap;
    int width;
    int height;


    Character(glm::vec3 startPosition, float startSpeed, float* heightmap, int width, int height);
    void move(GLFWwindow* window);
    float barycentricCoordinates(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
    void getTriangleForPosition(float x, float z, glm::vec3& a, glm::vec3& b, glm::vec3& c);
};



