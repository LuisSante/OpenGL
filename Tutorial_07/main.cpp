#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <deque>
#include <cmath>

typedef unsigned int ui;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const ui SCR_WIDTH = 800;
const ui SCR_HEIGHT = 600;
const double pi = 3.1415;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tutorial 07", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ui vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ui fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    std::deque <ui> deque_shaderProgram;
    for (int i = 0; i < 7; i++) {
        ui shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        deque_shaderProgram.push_back(shaderProgram);
    }

    glDeleteShader(vertexShader);

    const float re = 0.7;
    const float partes_triangles = 7;
    const float p = pi * 2 / partes_triangles;

    float container_vertices[7][9];
    float size_vertices[9];
    double inclinacion = 0;

    for (int i = 0; i < partes_triangles; i++) {
        container_vertices[i][0] = 0.0f;//origen
        container_vertices[i][1] = 0.0f;//origen
        container_vertices[i][2] = 0.0f;//origen
        container_vertices[i][3] = float(cos(inclinacion) * re);//vertices de afuera
        container_vertices[i][4] = float(sin(inclinacion) * re);//vertices de afuera
        container_vertices[i][5] = 0.0f;
        inclinacion = inclinacion + p;//aumentar el angulo para encontrar el otro vertice
        container_vertices[i][6] = float(cos(inclinacion) * re);//vertices de afuera
        container_vertices[i][7] = float(sin(inclinacion) * re);//vertices de afuera
        container_vertices[i][8] = 0.0f;
    }

    ui indices[] = { 0, 1, 2 };
    ui VBO[7], VAO[7], EBO;
    glGenVertexArrays(7, VAO);
    glGenBuffers(7, VBO);
    glGenBuffers(1, &EBO);


    for (int i = 0; i < partes_triangles; i++) {
        glBindVertexArray(VAO[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(size_vertices), container_vertices[i], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int time = 0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        for (int i = 0; i < partes_triangles; i++) {
            glUseProgram(deque_shaderProgram[i]);
            glBindVertexArray(VAO[i]);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        }

        if (time == 150) {
            deque_shaderProgram.push_back(deque_shaderProgram.front());
            deque_shaderProgram.pop_front();

            double  timeValue = glfwGetTime();
            float colorValue = static_cast<float>(abs(sin(timeValue) / 2.0 + 0.5));
            float colorValue2 = static_cast<float>(abs(cos(timeValue) / 2.0 + 0.5));
            float colorValue3 = static_cast<float>(abs(tan(timeValue) / 2.0 + 0.5));

            float list_color[] = {colorValue,colorValue2,colorValue3};
            int cont = 0;
            int pos = 0;
            for(int i=0;i<3;i++){
                int vertexColorLocation = glGetUniformLocation(deque_shaderProgram[i], "ourColor");
                if (cont == 0) {
                    glUniform4f(vertexColorLocation, 0.0f, 0.0f, list_color[pos], 1.0f);
                    glUniform4f(vertexColorLocation, 0.0f, 0.0f, list_color[pos+1], 1.0f);
                    glUniform4f(vertexColorLocation, 0.0f, 0.0f, list_color[pos+2], 1.0f);
                }

                if (cont == 1) {
                    glUniform4f(vertexColorLocation, 0.0f, list_color[pos], list_color[pos+1], 1.0f);
                    glUniform4f(vertexColorLocation, 0.0f, list_color[pos+1], list_color[pos+2], 1.0f);
                    glUniform4f(vertexColorLocation, 0.0f, list_color[pos], list_color[pos+2], 1.0f);
                }

                if (cont == 2) {
                    glUniform4f(vertexColorLocation, list_color[pos], 0.0f, list_color[pos+2], 1.0f);
                    glUniform4f(vertexColorLocation, list_color[pos], 0.0f, list_color[pos + 1], 1.0f);
                    glUniform4f(vertexColorLocation, list_color[pos+1], 0.0f, list_color[pos + 2], 1.0f);
                }

                if (cont == 3) {
                    glUniform4f(vertexColorLocation, list_color[pos], list_color[pos+1], list_color[pos + 2], 1.0f);
                    cont = 0;
                }
                cont++;
            }

            time = 0;
        }
        time++;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(7, VAO);
    glDeleteBuffers(7, VBO);
    glDeleteBuffers(1, &EBO);
    deque_shaderProgram.clear();
    deque_shaderProgram.shrink_to_fit();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}