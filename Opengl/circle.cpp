#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

const char* vs = "#version 330 core\nlayout (location=0) in vec3 aPos;\nvoid main(){gl_Position = vec4(aPos,1.0);}";

const char* fs = "#version 330 core\nout vec4 color;\nvoid main(){color = vec4(1.0,0.0,0.0,1.0);}";

int main() {
    glfwInit();
    GLFWwindow* w = glfwCreateWindow(800,600,"Circle",NULL,NULL);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    const int n = 10; // number of points (more = smoother)
    float v[(n+2)*3];

    // center
    v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f;

    for(int i=0;i<=n;i++){
        float angle = 2.0f * 3.14159f * i / n;
        v[(i+1)*3 + 0] = 0.5f * cos(angle);
        v[(i+1)*3 + 1] = 0.5f * sin(angle);
        v[(i+1)*3 + 2] = 0.0f;
    }

    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vsID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsID,1,&vs,NULL);
    glCompileShader(vsID);

    unsigned int fsID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsID,1,&fs,NULL);
    glCompileShader(fsID);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog,vsID);
    glAttachShader(prog,fsID);
    glLinkProgram(prog);

    while(!glfwWindowShouldClose(w)){
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN,0,n+2); // 🔵 draw circle

        glfwSwapBuffers(w);
        glfwPollEvents();
    }

    glfwTerminate();
}