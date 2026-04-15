#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Buffer Example", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while (!glfwWindowShouldClose(window)) {
        
        // 🔵 Draw to BACK BUFFER
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Red color
        glClear(GL_COLOR_BUFFER_BIT);

        // 🔁 Swap buffers (BACK → FRONT)
        glfwSwapBuffers(window);
       

        // Handle input/events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}