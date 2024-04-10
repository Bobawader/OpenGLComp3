#include "character.h"

Character::Character(glm::vec3 startPosition, float startSpeed)
    : position(startPosition), speed(startSpeed) {}

//void Character::move(GLFWwindow* window, std::vector<std::vector<Vertex>>& surfaceVertices, int width, int height) {
//    float speed = 0.01f; // Define a speed for the movement
//    float boundary = 0.5f; // Define the boundary of the plane
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && position.z - speed > -boundary)
//        position.z -= speed; // Move forward
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && position.z + speed < boundary)
//        position.z += speed; // Move backward
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && position.x - speed > -boundary)
//        position.x -= speed; // Move left
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && position.x + speed < boundary)
//        position.x += speed; // Move right
//
//     //Ensure the character stays within the bounds of the surface.
//    position.x = glm::clamp(position.x, -0.5f, 0.5f);
//    position.z = glm::clamp(position.z, -0.5f, 0.5f);
//
//    // Update the character's y position based on the surface height at the new x, z position.
//    float x = (position.x + 0.5f) * (width - 1);  // Map x from [-0.5, 0.5] to [0, width - 1]
//    float z = (position.z + 0.5f) * (height - 1); // Map z from [-0.5, 0.5] to [0, height - 1]
//    glm::vec3 p = position; // The character's current position
//    glm::vec3 a = surfaceVertices[(int)x][(int)z].position; // The position of the first vertex of the triangle
//    glm::vec3 b = surfaceVertices[(int)x + 1][(int)z].position; // The position of the second vertex of the triangle
//    glm::vec3 c = surfaceVertices[(int)x][(int)z + 1].position; // The position of the third vertex of the triangle
//    glm::vec3 bary = barycentricCoordinates(p, a, b, c);
//
//    // Interpolate the y position using the barycentric coordinates
//    position.y = bary.x * a.y + bary.y * b.y + bary.z * c.y;
//}


//void Character::move(GLFWwindow* window, std::vector<std::vector<Vertex>>& surfaceVertices, int width, int height) {
//    float speed = 0.001f; // Define a speed for the movement
//    //float boundary = 0.5f; // Define the boundary of the plane
//
//
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && position.z - speed > -0.5f)
//        position.z -= speed; // Move forward
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && position.z + speed < 0.5f)
//        position.z += speed; // Move backward
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && position.x - speed > -0.5f)
//        position.x -= speed; // Move left
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        position.x += speed;
//
//    
//
//    //Ensure the character stays within the bounds of the surface.
//    position.x = glm::clamp(position.x, -9.5f, 3.5f);
//    position.z = glm::clamp(position.z, -6.5f, 6.5f);
//
//
//    // Update the character's y position based on the surface height at the new x, z position.
//    float x = (position.x + 0.5f) * (width - 1);  // Map x from [-0.5, 0.5] to [0, width - 1]
//    float z = (position.z + 0.5f) * (height - 1); // Map z from [-0.5, 0.5] to [0, height - 1]
//    glm::vec3 p = glm::vec3(x, position.y, z);
//
//    
//    if (x < width - 2 && z < height - 2) {
//        glm::vec3 a = surfaceVertices[(int)x][(int)z].position;
//        glm::vec3 b = surfaceVertices[(int)x + 1][(int)z].position;
//        glm::vec3 c = surfaceVertices[(int)x][(int)z + 1].position;
//        float bary = barycentricCoordinates(p, a, b, c);
//
//        // Always update the y position using the barycentric coordinates
//        position.y = bary;
//    }
//}


void Character::move(GLFWwindow* window, std::vector<std::vector<Vertex>>& surfaceVertices, int width, int height) {
    float speed = 0.001f; // Define a speed for the movement

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && position.z - speed > -0.5f)
        position.z -= speed; // Move forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && position.z + speed < 0.5f)
        position.z += speed; // Move backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && position.x - speed > -0.5f)
        position.x -= speed; // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position.x += speed; // Move right

    // Ensure the character stays within the bounds of the surface.
    position.x = glm::clamp(position.x, -4.5f, 5.5f);
    position.z = glm::clamp(position.z, -6.5f, 6.5f);

    // Calculate the position on the surface
    float x = (position.x + 4.5f) / 10.0f * (width - 1); 
    float z = (position.z + 6.5f) / 13.0f * (height - 1); 

    if (x >= 0 && x < width - 1 && z >= 0 && z < height - 1) {
        int ix = static_cast<int>(x);
        int iz = static_cast<int>(z);
        glm::vec3 a = surfaceVertices[ix][iz].position;
        glm::vec3 b = surfaceVertices[ix + 1][iz].position;
        glm::vec3 c = surfaceVertices[ix][iz + 1].position;

        // Calculate barycentric coordinates for the current position
        glm::vec3 p = glm::vec3(x - ix, 0.0f, z - iz); // Local coordinates within the triangle
        float bary = barycentricCoordinates(p, a, b, c);

        // Update character's y position based on the surface height
        float newY = a.y + bary * (b.y - a.y) + (1.0f - bary) * (c.y - a.y);
        position.y = newY;
    }
}



float Character::barycentricCoordinates(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float baryx = (d11 * d20 - d01 * d21) / denom;
    float baryy = (d00 * d21 - d01 * d20) / denom;
    float baryz = 1.0f - baryx - baryy;
    return baryx * a.y + baryy * b.y + baryz * c.y; // Interpolated y-coordinate
}










