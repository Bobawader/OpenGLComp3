#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Obj {
public:

//Obj(glm::vec3 position, glm::vec3 orientation, unsigned int texture);
    


    glm::vec3 position;
    glm::vec3 orientation;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    unsigned int texture;
    unsigned int VAO, VBO, EBO;
};
