#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Rotation
float yaw = 0.0f, pitch = 0.0f;

// Position
float posX = 0.0f, posY = 0.0f;

// Mouse
float lastX = 400, lastY = 300;
bool firstMouse = true;
bool leftPressed = false;
bool rightPressed = false;

// Mouse button
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        leftPressed = (action == GLFW_PRESS);

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        rightPressed = (action == GLFW_PRESS);

    if (action == GLFW_RELEASE)
        firstMouse = true;
}

// Mouse movement
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!leftPressed && !rightPressed) return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.3f;

    // Rotate
    if (leftPressed)
    {
        yaw += xoffset * sensitivity;
        pitch += yoffset * sensitivity;
    }

    // Move
    if (rightPressed)
    {
        posX += xoffset * 0.005f;
        posY += yoffset * 0.005f;
    }
}

// Pyramid vertices
float vertices[] = {
    // Base
    -0.5f, 0.0f, -0.5f,
     0.5f, 0.0f, -0.5f,
     0.5f, 0.0f,  0.5f,

     0.5f, 0.0f,  0.5f,
    -0.5f, 0.0f,  0.5f,
    -0.5f, 0.0f, -0.5f,

    // Sides
    -0.5f, 0.0f, -0.5f,
     0.5f, 0.0f, -0.5f,
     0.0f, 0.8f,  0.0f,

     0.5f, 0.0f, -0.5f,
     0.5f, 0.0f,  0.5f,
     0.0f, 0.8f,  0.0f,

     0.5f, 0.0f,  0.5f,
    -0.5f, 0.0f,  0.5f,
     0.0f, 0.8f,  0.0f,

    -0.5f, 0.0f,  0.5f,
    -0.5f, 0.0f, -0.5f,
     0.0f, 0.8f,  0.0f
};

// Shaders
const char* vs = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fs = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Rotate & Move Pyramid", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glEnable(GL_DEPTH_TEST);

    // Shader setup
    int v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, NULL);
    glCompileShader(v);

    int f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, NULL);
    glCompileShader(f);

    int program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);

    glDeleteShader(v);
    glDeleteShader(f);

    // VAO + VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        // MODEL (move + rotate)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(posX, posY, 0.0f));
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(yaw), glm::vec3(0, 1, 0));

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f/600.0f,
                                                0.1f,
                                                100.0f);

        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}