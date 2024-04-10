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



int main()
{
    Camera camera;
    Vertex vertex;
    glm::vec3 startPosition = glm::vec3(1.0f, 0.0f, 0.0f); 
    Character character(startPosition, 0.01f);
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

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Texture/container2.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //glm::vec3 position(0.0f, 0.0f, 0.0f);
    //glm::vec3 orientation(0.0f, 0.0f, 0.0f);
    ////unsigned int texture;
    ////glGenTextures(1, &texture);

    //Obj obj(position, orientation, texture);

    

    const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)glsl";


    const char* vertexShaderTextureSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoords = aTexCoords;
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
in vec3 ourColor;
in vec2 TexCoords;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
    vec3 texColor = texture(ourTexture, TexCoords).rgb;
    FragColor = vec4(ourColor * texColor, 1.0f);
}

)glsl";


    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader);

    unsigned int vertexShaderTexture = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderTexture, 1, &vertexShaderTextureSource, nullptr);
    glCompileShader(vertexShaderTexture);
    checkShaderCompilation(vertexShaderTexture);

    // Compile the fragment shader
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

        





float vertices[] = {
    // Lower left corner
    -4.5f, -0.5f, -6.5f,
    // Lower right corner
    7.5f, -0.5f, -6.5f,
    // Upper right corner
    7.5f, -0.5f,  6.5f,
    // Upper left corner
    -4.5f, -0.5f,  6.5f,
    // Top corner, raised
    0.5f, 5.0f, 0.0f
};


    // Define the indices of the triangles
    unsigned int indices[] = {
        0, 1, 4,  
        1, 2, 4, 
        2, 3, 4,  
        3, 0, 4   
    };

    unsigned int surfaceVAO, surfaceVBO, surfaceEBO;
    glGenVertexArrays(1, &surfaceVAO);
    glBindVertexArray(surfaceVAO);

    glGenBuffers(1, &surfaceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, surfaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &surfaceEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfaceEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    

    std::vector<std::vector<Vertex>> surfaceVertices;
    for (int i = 0; i < 100; ++i) {
        std::vector<Vertex> row;
        for (int j = 0; j < 100; ++j) {
            Vertex v;
            v.position = glm::vec3(i / 100.0f - 0.5f, 0, j / 100.0f - 0.5f); // Example position
            row.push_back(v);
        }
        surfaceVertices.push_back(row);
    }

    
    



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


    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesObj), verticesObj, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesObj), indicesObj, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




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



    while (!glfwWindowShouldClose(window)) {
        
        camera.inputs(window);
        character.move(window, surfaceVertices, 100, 100);
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

        
        //draw object with texture
        glUseProgram(shaderProgramTexture);
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 cubePosition = glm::vec3(0.0f, 0.4f, 0.5f); 
        model = glm::translate(model, cubePosition); 
        float angle = glm::radians(40.0f); 
        glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);
        model = glm::rotate(model, angle, axis);
        model = glm::scale(model, glm::vec3(0.5f));
        GLint modelLoc = glGetUniformLocation(shaderProgramTexture, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgramTexture, "view");
        GLint projLoc = glGetUniformLocation(shaderProgramTexture, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);





        //Draw surface
        glUseProgram(shaderProgram);
        
        glm::mat4 modelSurf = glm::mat4(1.0f);
        modelSurf = glm::scale(modelSurf, glm::vec3(0.1f)); // Scale the cube as needed
        GLint modelLocSurf = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLocSurf, 1, GL_FALSE, glm::value_ptr(modelSurf));
        GLint viewLocSurf = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLocSurf, 1, GL_FALSE, glm::value_ptr(view));
        GLint projLocSurf = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLocSurf, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(surfaceVAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);


        // Render Cube
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

