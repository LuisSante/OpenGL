#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <queue>

typedef unsigned int ui;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const double pi = 3.1415;
const ui SCR_WIDTH = 800;
const ui SCR_HEIGHT = 800;
const int colors_size = 7;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.77, 0.02, 0.02, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.86, 0.02, 0.02, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource3 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.98, 0.11, 0.11, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource4 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.97, 0.18, 0.18, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource5 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.00, 0.32, 0.32, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource6 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.00, 0.51, 0.51, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource7 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.00, 0.91, 0.91, 1.0);\n"
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tutorial 06", NULL, NULL);
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

    const GLchar* list_FragmentShaderSource[] = {fragmentShaderSource,
        fragmentShaderSource2, fragmentShaderSource3, fragmentShaderSource4,
        fragmentShaderSource5, fragmentShaderSource6, fragmentShaderSource7
    };

    std::vector<ui> list_fragmentShader;
    for (int i = 0; i < colors_size; i++) {
        ui fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &list_FragmentShaderSource[i], NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        list_fragmentShader.push_back(fragmentShader);
    }

    std::deque <ui> deque_shaderProgram;
    for (int i = 0; i < colors_size; i++) {
        ui shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, list_fragmentShader[i]);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        deque_shaderProgram.push_back(shaderProgram);
    }

    glDeleteShader(vertexShader);
    list_fragmentShader.clear();
    list_fragmentShader.shrink_to_fit();

    const float re = 0.7;
    const float partes_triangles = 7;
    const float p = pi * 2 / partes_triangles;

    float container_vertices[colors_size][9];
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

    ui indices[] = {0, 1, 2};
    ui VBO[colors_size], VAO[colors_size], EBO;
    glGenVertexArrays(colors_size, VAO);
    glGenBuffers(colors_size, VBO);
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
    int swap = 0;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < partes_triangles; i++) {
            glUseProgram(deque_shaderProgram[i]);
            glBindVertexArray(VAO[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        if (time == 90) {
            deque_shaderProgram.push_back(deque_shaderProgram.front());
            deque_shaderProgram.pop_front();
            time = 0;
        }
        time++;
        swap++;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(colors_size, VAO);
    glDeleteBuffers(colors_size, VBO);
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