#include "character.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "Vertex.h"


Character::Character(glm::vec3 startPosition, float startSpeed, float* heightmap, int width, int height)
    : position(startPosition), speed(startSpeed), heightmap(heightmap), width(width), height(height) {
}


void Character::move(GLFWwindow* window) {
    float speed = 0.001f; // Adjust movement speed as needed

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position.z -= speed; // Move forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position.z += speed; // Move backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position.x -= speed; // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position.x += speed; // Move right

    // Update the character's height based on the terrain
    glm::vec3 a, b, c;
    getTriangleForPosition(position.x, position.z, a, b, c);
    position.y = barycentricCoordinates(position, a, b, c);

    std::cout << position.y << std::endl;
}

float Character::barycentricCoordinates(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 v0 = b - a;
    glm::vec3 v1 = c - a;
    glm::vec3 v2 = p - a;

    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;

    float baryx = (d11 * d20 - d01 * d21) / denom;
    float baryy = (d00 * d21 - d01 * d20) / denom;
    float baryz = 1.0f - baryx - baryy;

    // Interpolate the y-coordinate based on barycentric coordinates
    float interpolatedY = baryx * a.y + baryy * b.y + baryz * c.y;

    //std::cout << interpolatedY << std::endl;

    return interpolatedY + 0.5f;
}

void Character::getTriangleForPosition(float x, float z, glm::vec3& a, glm::vec3& b, glm::vec3& c) {
    // Determine the grid indices based on character position
    int i = static_cast<int>(x + width/2.0f);
    int j = static_cast<int>(z + height / 2.0f);
    

    // Check bounds to avoid accessing out-of-range indices
    if (i < 0 || i >= width - 1 || j < 0 || j >= height - 1) {
        // Handle out-of-bounds case (e.g., return default triangle)
        a = glm::vec3(0.0f, 0.0f, 0.0f);
        b = glm::vec3(0.0f, 0.0f, 0.0f);
        c = glm::vec3(0.0f, 0.0f, 0.0f);
        return;
    }

    // Retrieve vertices of the triangle in the heightmap grid using pointer arithmetic
    float* base = heightmap + (i * height + j); // Pointer to base of heightmap array
    a = glm::vec3(i - width / 2.0f, *base, j - height / 2.0f);
    b = glm::vec3(i + 1 - width / 2.0f, *(base + 1), j - height / 2.0f);
    c = glm::vec3(i - width / 2.0f, *(base + height), j + 1 - height / 2.0f);

    //std::cout << c.y << std::endl;
}


//void Character::move(GLFWwindow* window) {
//    float speed = 0.001f; // Define a speed for the movement
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        position.z -= speed; // Move forward
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        position.z += speed; // Move backward
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        position.x -= speed; // Move left
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        position.x += speed; // Move right
//
//    // Get the vertices of the triangle at the character's position
//    glm::vec3 a, b, c;
//    getTriangleForPosition(position.x, position.z, a, b, c);
//
//    // Adjust the y-coordinate based on the height of the surface
//    position.y = barycentricCoordinates(position, a, b, c);
//}






//float Character::barycentricCoordinates(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
//    glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
//    float d00 = glm::dot(v0, v0);
//    float d01 = glm::dot(v0, v1);
//    float d11 = glm::dot(v1, v1);
//    float d20 = glm::dot(v2, v0);
//    float d21 = glm::dot(v2, v1);
//    float denom = d00 * d11 - d01 * d01;
//    float baryx = (d11 * d20 - d01 * d21) / denom;
//    float baryy = (d00 * d21 - d01 * d20) / denom;
//    float baryz = 1.0f - baryx - baryy;
//    return baryx * a.y + baryy * b.y + baryz * c.y; // Interpolated y-coordinate
//}





//void Character::getTriangleForPosition(float x, float z, glm::vec3& a, glm::vec3& b, glm::vec3& c) {
//    // Convert the x and z coordinates to indices in the heightmap array
//    int i = (int)(x + width / 2.0f);
//    int j = (int)(z + height / 2.0f);
//    
//
//    // Check if the position is within the bounds of the heightmap
//    if (i < 0 || i >= width || j < 0 || j >= height) {
//        // The position is out of bounds, return a default triangle
//        a = glm::vec3(0, 0, 0);
//        b = glm::vec3(0, 0, 0);
//        c = glm::vec3(0, 0, 0);
//        return;
//    }
//
//    // Get the four corners of the cell in the heightmap
//    glm::vec3 topLeft = glm::vec3(i - width / 2.0f, heightmap[i * width + j], j - height / 2.0f);
//    glm::vec3 topRight = glm::vec3((i + 1) - width / 2.0f, heightmap[(i + 1) * width + j], j - height / 2.0f);
//    glm::vec3 bottomLeft = glm::vec3(i - width / 2.0f, heightmap[i * width + j + 1], (j + 1) - height / 2.0f);
//    glm::vec3 bottomRight = glm::vec3((i + 1) - width / 2.0f, heightmap[(i + 1) * width + j + 1], (j + 1) - height / 2.0f);
//
//    // Determine which triangle the position is in
//    if ((x + width / 2.0f) - i < (z + height / 2.0f) - j) {
//        // The position is in the top-left triangle of the cell
//        a = topLeft;
//        b = topRight;
//        c = bottomLeft;
//    }
//    else {
//        // The position is in the bottom-right triangle of the cell
//        a = bottomRight;
//        b = topRight;
//        c = bottomLeft;
//    }
//}



















