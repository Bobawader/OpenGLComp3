#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#include <iostream>
#include <cstdlib>  
#include <ctime>
#include <vector>
#include <cmath>
#include "stb_image.h"
#include "Camera.h"
#include "Vertex.h"
#include "character.h"
#include "Obj.h"
#include "NPC.h"
#include "Parametric.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void checkShaderCompilation(GLuint shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

// Function to check program linking
void checkProgramLinking(GLuint program) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

const int width = 100;
const int height = 100;

float heightmap[width][height];


void createHeightmap() {
    // Fill the heightmap with values
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Use a sine wave function to create "bumps"
            float heightValue = 1.0f * (sin(i / 10.0f) + cos(j / 10.0f));
            heightmap[i][j] = heightValue;

          
        }
    }
}








int main()
{

    createHeightmap();

    Camera camera;
    Vertex vertex;
    glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f); 
    float startSpeed = 0.01f;
    /*Character character(startPosition, startSpeed, heightmap, width, height);*/
    Character character(startPosition, startSpeed, &heightmap[0][0], width, height);
    Parametric parametric;
    


    

    



    glfwInit();

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    

    

    const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
)glsl";


    




    

    const char* fragmentShaderSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    //FragColor = vec4(ourColor, 1.0f);
    FragColor = vec4(0.5f, 1.0f, 0.0f, 1.0f);
}
)glsl";

    const char* fragmentShaderCharacterSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)glsl";

    const char* fragmentShaderParaSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)glsl";


    const char* fragmentShaderNPCSource = R"glsl(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)glsl";



    const char* fragmentShaderObjSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main()
{
FragColor = texture(ourTexture, TexCoord);
}

)glsl";


    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader);

    unsigned int vertexShaderTexture = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderTexture, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderTexture);
    checkShaderCompilation(vertexShaderTexture);

    // Texture Obj
    unsigned int fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderObj, 1, &fragmentShaderObjSource, nullptr);
    glCompileShader(fragmentShaderObj);
    checkShaderCompilation(fragmentShaderObj);

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader);

    //Parametric curve
    unsigned int fragmentShaderPara = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderPara, 1, &fragmentShaderParaSource, nullptr);
    glCompileShader(fragmentShaderPara);
    checkShaderCompilation(fragmentShaderPara);

    //NPC
    unsigned int fragmentShaderNPC = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderNPC, 1, &fragmentShaderNPCSource, nullptr);
    glCompileShader(fragmentShaderNPC);
    checkShaderCompilation(fragmentShaderNPC);

    //Character
    unsigned int fragmentShaderCharacter = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderCharacter, 1, &fragmentShaderCharacterSource, nullptr);
    glCompileShader(fragmentShaderCharacter);
    checkShaderCompilation(fragmentShaderCharacter);

    // Shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    //Parametric shader
    unsigned int shaderProgramPara = glCreateProgram();
    glAttachShader(shaderProgramPara, vertexShader);
    glAttachShader(shaderProgramPara, fragmentShaderPara);
    glLinkProgram(shaderProgramPara);
    checkProgramLinking(shaderProgramPara);


    //NPC shader
    unsigned int shaderProgramNPC = glCreateProgram();
    glAttachShader(shaderProgramNPC, vertexShader);
    glAttachShader(shaderProgramNPC, fragmentShaderNPC);
    glLinkProgram(shaderProgramNPC);
    checkProgramLinking(shaderProgramNPC);

    //Character shader
    unsigned int shaderProgramCharacter = glCreateProgram();
    glAttachShader(shaderProgramCharacter, vertexShader);
    glAttachShader(shaderProgramCharacter, fragmentShaderCharacter);
    glLinkProgram(shaderProgramCharacter);
    checkProgramLinking(shaderProgramCharacter);

    

    // Obj shader
    unsigned int shaderProgramTexture = glCreateProgram();
    glAttachShader(shaderProgramTexture, vertexShaderTexture);
    glAttachShader(shaderProgramTexture, fragmentShaderObj);
    glLinkProgram(shaderProgramTexture);
    checkProgramLinking(shaderProgramTexture);


    glDeleteShader(vertexShaderTexture);
    glDeleteShader(fragmentShaderObj);
    glDeleteShader(fragmentShaderPara);
    glDeleteShader(fragmentShaderNPC);
    glDeleteShader(fragmentShaderCharacter);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderCharacter);

    glEnable(GL_DEPTH_TEST);

        




   


    float scale_factor = 0.1f;
    float vertices[width * height * 3];
    unsigned int indices[(width - 1) * (height - 1) * 6];
    int vi = 0, ii = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            
            vertices[vi++] = scale_factor * (i - width / 2.0f); // Scale the x-coordinate
            vertices[vi++] = scale_factor * heightmap[i][j]; // Keep the y-coordinate as it is
            vertices[vi++] = scale_factor * (j - height / 2.0f);
            

            // Add indices for the square formed by this point and its right and bottom neighbors
            if (i < width - 1 && j < height - 1) {
                int topLeft = i * height + j;
                int topRight = topLeft + 1;
                int bottomLeft = topLeft + height;
                int bottomRight = bottomLeft + 1;

                indices[ii++] = topLeft;
                indices[ii++] = bottomLeft;
                indices[ii++] = topRight;

                indices[ii++] = topRight;
                indices[ii++] = bottomLeft;
                indices[ii++] = bottomRight;
            }
        }
    }

    float heightmap[width][height];

    // Traverse through the vertices defined by the indices
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // Calculate the vertex index corresponding to the current heightmap position
            int vertexIndex = i * height + j;

            // Sample the y-coordinate (height) from the vertex data
            float yPos = vertices[vertexIndex * 3 + 1]; // Assuming y-coordinate is stored every 3 elements

            // Update the heightmap with the sampled height
            heightmap[i][j] = yPos;
        }
    }

    // Create and bind the VAO
    unsigned int surfaceVAO;
    glGenVertexArrays(1, &surfaceVAO);
    glBindVertexArray(surfaceVAO);

    // Create and bind the VBO
    unsigned int surfaceVBO;
    glGenBuffers(1, &surfaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, surfaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and bind the EBO
    unsigned int surfaceEBO;
    glGenBuffers(1, &surfaceEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfaceEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    
    



    float cubeVertices[] = {
        // Front face
        -0.05f, -0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,
        // Back face
        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,
    };


    unsigned int cubeIndices[] = {
        0, 1, 2,  2, 3, 0,  // Front face
        1, 5, 6,  6, 2, 1,  // Right face
        7, 6, 5,  5, 4, 7,  // Back face
        4, 0, 3,  3, 7, 4,  // Left face
        4, 5, 1,  1, 0, 4,  // Bottom face
        3, 2, 6,  6, 7, 3   // Top face
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cubeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    float verticesObj[] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
            };
        
            unsigned int indicesObj[] = {
            0, 1, 2,  // first triangle
            0, 2, 3,  // second triangle
            4, 5, 6,  // third triangle
            4, 6, 7,  // fourth triangle
            8, 9, 10,  // fifth triangle
            8, 10, 11,  // sixth triangle
            12, 13, 14,  // seventh triangle
            12, 14, 15,  // eighth triangle
            16, 17, 18,  // ninth triangle
            16, 18, 19,  // tenth triangle
            20, 21, 22,  // eleventh triangle
             20, 22, 23  // twelfth triangle
            };


    

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Texture/container2.jpg", &width, &height,
        &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glUseProgram(shaderProgramTexture);
    glUniform1i(glGetUniformLocation(shaderProgramTexture, "ourTexture"), 0);



    // Generate the VAO, VBO, and EBO
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesObj), verticesObj, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesObj), indicesObj, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

   


    



//curve


    std::vector<glm::vec3> curvePoints;
    for (float t = 0; t <= 1; t += 0.01f) {
        glm::vec3 a(vertices[0], vertices[1], vertices[2]);
        glm::vec3 b(vertices[3], vertices[4], vertices[5]);
        glm::vec3 c(vertices[6], vertices[7], vertices[8]);
        glm::vec3 pointOnSurface = parametric.mapPointToSurface(t, a, b, c);
        curvePoints.push_back(pointOnSurface);
    }



    unsigned int paraVBO;
    glGenBuffers(1, &paraVBO);
    glBindBuffer(GL_ARRAY_BUFFER, paraVBO);
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(glm::vec3), &curvePoints[0], GL_STATIC_DRAW);

    unsigned int paraVAO;
    glGenVertexArrays(1, &paraVAO);
    glBindVertexArray(paraVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);



    NPC npc(curvePoints);

    float NPCVertices[] = {
        // Front face
        -0.05f, -0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,
        // Back face
        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,
    };


    unsigned int NPCIndices[] = {
        0, 1, 2,  2, 3, 0,  // Front face
        1, 5, 6,  6, 2, 1,  // Right face
        7, 6, 5,  5, 4, 7,  // Back face
        4, 0, 3,  3, 7, 4,  // Left face
        4, 5, 1,  1, 0, 4,  // Bottom face
        3, 2, 6,  6, 7, 3   // Top face
    };

    unsigned int NPCVAO, NPCVBO, NPCEBO;
    glGenVertexArrays(1, &NPCVAO);
    glBindVertexArray(NPCVAO);

    glGenBuffers(1, &NPCVBO);
    glBindBuffer(GL_ARRAY_BUFFER, NPCVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(NPCVertices), NPCVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &NPCEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NPCEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(NPCIndices), NPCIndices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

   
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    //render

    while (!glfwWindowShouldClose(window)) {
        
        camera.inputs(window);
        character.move(window);
        //, surfaceVertices , 100, 100
        npc.updatePosition(curvePoints);
        

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.orientation, camera.up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);



        

        // Draw the NPC
        glUseProgram(shaderProgramNPC);
        glm::mat4 modelNPC = glm::mat4(1.0f);
        modelNPC = glm::translate(modelNPC, npc.position);
        GLint modelLocNPC = glGetUniformLocation(shaderProgramNPC, "model");
        glUniformMatrix4fv(modelLocNPC, 1, GL_FALSE, glm::value_ptr(modelNPC));
        GLint viewLocNPC = glGetUniformLocation(shaderProgramNPC, "view");
        glUniformMatrix4fv(viewLocNPC, 1, GL_FALSE, glm::value_ptr(view));
        GLint projLocNPC = glGetUniformLocation(shaderProgramNPC, "projection");
        glUniformMatrix4fv(projLocNPC, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(NPCVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



       //Draw parametric curve
        glUseProgram(shaderProgramPara);
        glm::mat4 modelCurve = glm::mat4(1.0f);
        GLint modelLocCurve = glGetUniformLocation(shaderProgramPara, "model");
        GLint viewLocCurve = glGetUniformLocation(shaderProgramPara, "view");
        GLint projLocCurve = glGetUniformLocation(shaderProgramPara, "projection");
        glUniformMatrix4fv(modelLocCurve, 1, GL_FALSE, glm::value_ptr(modelCurve));
        glUniformMatrix4fv(viewLocCurve, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLocCurve, 1, GL_FALSE, glm::value_ptr(projection));

       
        glBindVertexArray(paraVAO);
        glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());

      
        
        glUseProgram(shaderProgramTexture);
        glm::mat4 modelObj = glm::mat4(1.0f);
        glm::vec3 objPosition = glm::vec3(0.0f, 0.4f, 0.5f); 
        modelObj = glm::translate(modelObj, objPosition);
        GLint modelLocObj = glGetUniformLocation(shaderProgramTexture, "model");
        glUniformMatrix4fv(modelLocObj, 1, GL_FALSE, glm::value_ptr(modelObj));
        GLint viewLocObj = glGetUniformLocation(shaderProgramTexture, "view");
        glUniformMatrix4fv(viewLocObj, 1, GL_FALSE, glm::value_ptr(view));
        GLint projLocObj = glGetUniformLocation(shaderProgramTexture, "projection");
        glUniformMatrix4fv(projLocObj, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        //Draw surface
        glUseProgram(shaderProgram);
        
        glm::mat4 modelSurf = glm::mat4(1.0f);
        //modelSurf = glm::scale(modelSurf, glm::vec3(0.1f)); 
        GLint modelLocSurf = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLocSurf, 1, GL_FALSE, glm::value_ptr(modelSurf));
        GLint viewLocSurf = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLocSurf, 1, GL_FALSE, glm::value_ptr(view));
        GLint projLocSurf = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLocSurf, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(surfaceVAO);
        //glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, 0);



        // Render Character
        glUseProgram(shaderProgramCharacter);
        glm::mat4 modelCube = glm::mat4(1.0f);
        modelCube = glm::translate(modelCube, character.position);
        GLint modelLocCube = glGetUniformLocation(shaderProgramCharacter, "model");
        glUniformMatrix4fv(modelLocCube, 1, GL_FALSE, glm::value_ptr(modelCube));
        GLint viewLocCube = glGetUniformLocation(shaderProgramCharacter, "view");
        glUniformMatrix4fv(viewLocCube, 1, GL_FALSE, glm::value_ptr(view));
        GLint projLocCube = glGetUniformLocation(shaderProgramCharacter, "projection");
        glUniformMatrix4fv(projLocCube, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 36 indices for a cube



        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &surfaceVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &NPCVAO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &paraVAO);
    glDeleteBuffers(1, &surfaceVBO);

    glfwTerminate();

    return 0;
}

