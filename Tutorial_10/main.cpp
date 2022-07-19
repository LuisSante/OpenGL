#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderCreator()
{
    const char* vertexShader = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    return vertexShader;
}

const char* fragmentShaderCreator(float R, float G, float B)
{
    // concat float to const char
    std::string temp = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(" + std::to_string(R) + "f, " + std::to_string(G) + "f, " + std::to_string(B) + "f, 1.0f);\n"
        "}\0";
    const char* fragmentShader = new char[temp.length() + 1];
    strcpy(const_cast<char*>(fragmentShader), temp.c_str());
    return fragmentShader;
}

const char* fragmentShaderUniformCreator()
{
    const char* fragmentShader = "#version 450 core\n"
        "out vec4 FragmentColor;\n"
        "uniform vec4 uniformColor;\n"
        "void main()\n"
        "{\n"
        "   FragmentColor = uniformColor;\n"
        "}\n\0";

    return fragmentShader;
}


const char* vertexShaderSource = vertexShaderCreator();

const char* fragmentShaderSource = fragmentShaderUniformCreator();
const char* fragmentShaderSource1 = fragmentShaderUniformCreator();
const char* fragmentShaderSource2 = fragmentShaderUniformCreator();
const char* fragmentShaderSource3 = fragmentShaderUniformCreator();
const char* fragmentShaderSource4 = fragmentShaderUniformCreator();
const char* fragmentShaderSource5 = fragmentShaderUniformCreator();
const char* fragmentShaderSource6 = fragmentShaderUniformCreator();
const char* fragmentShaderSource7 = fragmentShaderUniformCreator();

std::vector<const char*> fragmentShaderSourceList = { fragmentShaderSource, fragmentShaderSource1, fragmentShaderSource2, fragmentShaderSource3, fragmentShaderSource4, fragmentShaderSource5, fragmentShaderSource6, fragmentShaderSource7 };

void prepare_shader(unsigned int& vertexShader, unsigned int& fragmentShader, const char*& vertexShaderSource, const char*& fragmentShaderSource, unsigned int& shaderProgram)
{
    // vertex shader	
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void prepare_VB0_VAO_triangle(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void prepare_VB0_VAO_line(unsigned int& VBO, unsigned int& VAO, float vertices[], int size_of_vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}


void draw_line(unsigned int& shaderProgram, unsigned int& VAO)
{
    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_LINES, 0, 2);
    // glBindVertexArray(0); // no need to unbind it every time 
}

void draw_point(unsigned int& shaderProgram, unsigned int& VAO)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

void get_triangle_coords(float x1, float y1, float x2, float y2, float x3, float y3, float* vertices)
{
    vertices[0] = x1; 	vertices[1] = y1; 	vertices[2] = 0.0f;
    vertices[3] = x2; 	vertices[4] = y2; 	vertices[5] = 0.0f;
    vertices[6] = x3; 	vertices[7] = y3; 	vertices[8] = 0.0f;
}

void draw_triangle(unsigned int& shaderProgram, unsigned int& VAO)
{
    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time 
}

void drawUniformTriangle(unsigned int& shaderProgram, unsigned int& VAO, float R, float G, float B)
{
    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "uniformColor");
    glUniform4f(vertexColorLocation, R, G, B, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time 
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    unsigned int vertexShader, fragmentShader, shaderProgram;
    prepare_shader(vertexShader, fragmentShader, vertexShaderSource, fragmentShaderSource, shaderProgram);

    unsigned int vertexShader_, fragmentShader_, shaderProgram_;
    prepare_shader(vertexShader_, fragmentShader_, vertexShaderSource, fragmentShaderSource1, shaderProgram_);

    float vertices[] = { 0.0f , 0.5f , 0.2f , 0.2f };
    float vertices1[] = { 0.2f, 0.2f , 0.5f , 0.2f};
    float vertices2[] = { 0.5f , 0.2f , 0.3f,-0.2f};
    float vertices3[] = { 0.3f,-0.2f , 0.5f, -0.5f };
    float vertices4[] = { 0.5f, -0.5f , 0.0f ,-0.25f};
    float vertices5[] = { 0.0f ,-0.25f , -0.5f , -0.5f };
    float vertices6[] = { -0.5f , -0.5f , -0.3f ,-0.2f};
    float vertices7[] = { -0.3f ,-0.2f , -0.5f ,0.2f};
    float vertices8[] = { -0.5f ,0.2f , -0.2f , 0.2f};
    float vertices9[] = { -0.2f , 0.2f , 0.0f , 0.5f };

    
    unsigned int VBO, VAO, VBO1, VAO1, VBO2, VAO2, VBO3, VAO3, VBO4, VAO4, VBO5, VAO5, VBO6, VAO6, VBO7, VAO7, VBO8, VAO8, VBO9, VAO9;
    unsigned int VBO_, VAO_, VBO1_, VAO1_, VBO2_, VAO2_, VBO3_, VAO3_, VBO4_, VAO4_, VBO5_, VAO5_, VBO6_, VAO6_, VBO7_, VAO7_, VBO8_, VAO8_, VBO9_, VAO9_;
    
    prepare_VB0_VAO_line(VBO, VAO, vertices, sizeof(vertices));
    prepare_VB0_VAO_line(VBO1, VAO1, vertices1, sizeof(vertices1));
    prepare_VB0_VAO_line(VBO2, VAO2, vertices2, sizeof(vertices2));
    prepare_VB0_VAO_line(VBO3, VAO3, vertices3, sizeof(vertices3));
    prepare_VB0_VAO_line(VBO4, VAO4, vertices4, sizeof(vertices4));
    prepare_VB0_VAO_line(VBO5, VAO5, vertices5, sizeof(vertices5));
    prepare_VB0_VAO_line(VBO6, VAO6, vertices6, sizeof(vertices6));
    prepare_VB0_VAO_line(VBO7, VAO7, vertices7, sizeof(vertices7));
    prepare_VB0_VAO_line(VBO8, VAO8, vertices8, sizeof(vertices8));
    prepare_VB0_VAO_line(VBO9, VAO9, vertices9, sizeof(vertices9));

    prepare_VB0_VAO_triangle(VBO_, VAO_, vertices, sizeof(vertices));
    prepare_VB0_VAO_triangle(VBO1_, VAO1_, vertices1, sizeof(vertices1));
    prepare_VB0_VAO_triangle(VBO2_, VAO2_, vertices2, sizeof(vertices2));
    prepare_VB0_VAO_triangle(VBO3_, VAO3_, vertices3, sizeof(vertices3));
    prepare_VB0_VAO_triangle(VBO4_, VAO4_, vertices4, sizeof(vertices4));
    prepare_VB0_VAO_triangle(VBO5_, VAO5_, vertices5, sizeof(vertices5));
    prepare_VB0_VAO_triangle(VBO6_, VAO6_, vertices6, sizeof(vertices6));
    prepare_VB0_VAO_triangle(VBO7_, VAO7_, vertices7, sizeof(vertices7));
    prepare_VB0_VAO_triangle(VBO8_, VAO8_, vertices8, sizeof(vertices8));
    prepare_VB0_VAO_triangle(VBO9_, VAO9_, vertices9, sizeof(vertices9));


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int vertexColorLocation = glGetUniformLocation(shaderProgram_, "uniformColor");
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
        draw_triangle(shaderProgram_, VAO);
        draw_triangle(shaderProgram_, VAO1);
        draw_triangle(shaderProgram_, VAO2);
        draw_triangle(shaderProgram_, VAO3);
        draw_triangle(shaderProgram_, VAO4);
        draw_triangle(shaderProgram_, VAO5);
        draw_triangle(shaderProgram_, VAO6);
        draw_triangle(shaderProgram_, VAO7);
        draw_triangle(shaderProgram_, VAO8);
        draw_triangle(shaderProgram_, VAO9);

        draw_line(shaderProgram, VAO);
        draw_line(shaderProgram, VAO1);
        draw_line(shaderProgram, VAO2);
        draw_line(shaderProgram, VAO3);
        draw_line(shaderProgram, VAO4);
        draw_line(shaderProgram, VAO5);
        draw_line(shaderProgram, VAO6);
        draw_line(shaderProgram, VAO7);
        draw_line(shaderProgram, VAO8);
        draw_line(shaderProgram, VAO9);
        draw_point(shaderProgram, VAO);
        draw_point(shaderProgram, VAO1);
        draw_point(shaderProgram, VAO2);
        draw_point(shaderProgram, VAO3);
        draw_point(shaderProgram, VAO4);
        draw_point(shaderProgram, VAO5);
        draw_point(shaderProgram, VAO6);
        draw_point(shaderProgram, VAO7);
        draw_point(shaderProgram, VAO8);
        draw_point(shaderProgram, VAO9);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

   

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}