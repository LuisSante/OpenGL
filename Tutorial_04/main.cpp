#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw3.lib")

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform int flag = 1;\n"
"void main(){\n"
"if(flag == 1)\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"else if(flag == 2)\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"else if(flag == 3)\n"
"	FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);\n"
"else if(flag == 4)\n"
"	FragColor = vec4(1.0f, 0.0f, 1.0f, 0.0f);\n"
"else if(flag == 5)\n"
"	FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
"else if(flag == 6)\n"
"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";



int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}


	// Cree un objeto de búfer de vértices para administrar el espacio de búfer de vértices asignado
	 // Crear un objeto sombreador
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Crea un sombreador de fragmentos
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Crear programa de sombreado
	unsigned int shaderProgram = glCreateProgram();
	// sombreador de enlaces
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	float vertices[] = {
		0.0f, 0.0f, 0.0f,//0
		0.8f, 0.0f, 0.0f,//1
		0.4f, 0.5f, 0.0f,//2

		0.0f, 0.0f, 0.0f,//0
		0.4f, 0.5f, 0.0f,//2
		-0.4f, 0.5f, 0.0f,//3

		0.0f, 0.0f, 0.0f,//0
		-0.4f, 0.5f, 0.0f,//3
		-0.8f, 0.0f, 0.0f,//4

		0.0f, 0.0f, 0.0f,//0
		-0.8f, 0.0f, 0.0f,//4
		-0.4f, -0.5f, 0.0f,//5

		 0.0f, 0.0f, 0.0f,//0
		 -0.4f, -0.5f, 0.0f,//5
		 0.4f, -0.5f, 0.0f,//6

		 0.0f, 0.0f, 0.0f,//0
		 0.4f, -0.5f, 0.0f,//6
		 0.8f, 0.0f, 0.0f//1
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO); // Crear un objeto de matriz de vértices
	glGenBuffers(1, &VBO); // Crear objetos de búfer de vértices


	// Copie la matriz de vértices en el búfer para usar OpenGL
	glBindVertexArray(VAO); // Vincula la matriz de vértices al objeto de matriz de vértices
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vincula el búfer de vértices al objeto de búfer de vértices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Datos de vértice de triángulo de búfer
	// Establecer puntero de atributo de vértice
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		int flagLoc = glGetUniformLocation(shaderProgram, "flag");
		glUniform1i(flagLoc, 1);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUniform1i(flagLoc, 2);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		glBindVertexArray(1);

		glUniform1i(flagLoc, 3);
		glDrawArrays(GL_TRIANGLES, 6, 3);
		glBindVertexArray(2);

		glUniform1i(flagLoc, 4);
		glDrawArrays(GL_TRIANGLES, 9, 3);
		glBindVertexArray(3);

		glUniform1i(flagLoc, 5);
		glDrawArrays(GL_TRIANGLES, 12, 3);
		glBindVertexArray(4);

		glUniform1i(flagLoc, 6);
		glDrawArrays(GL_TRIANGLES, 15, 3);
		glBindVertexArray(5);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}