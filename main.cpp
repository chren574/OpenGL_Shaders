// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

#include <iostream> // std::cout

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;

int main()
{
	/* ----------------------------------- INIT WINDOW -----------------------------------*/

	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Newest supported OpenGL version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Oldest supported OpenGL version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Errors if legacy code is used
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Not resizeble by the user
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback); // Set callback function for input

	// Setting glewExperimental to true ensures GLEW uses more modern techniques for managing OpenGL functionality. 
	// Leaving it to its default value of GL_FALSE might give issues when using the core profile of OpenGL.
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	/* ----------------------------------- SHADERS -----------------------------------*/

	Shader ourShader("Shaders/shader.vert", "Shaders/shader.frag"); // Build and compile our shader program

	GLfloat vertices[] = {
		// Positions         // Colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 2, 1  // First Triangle
	};

	/* ----------------------------------- BUFFERS -----------------------------------*/
	
	// Vertex Buffer Object, Vertex Array Object, Element Buffer Objects
	GLuint VAO, VBO, EBO;

	// Creates IDs for the buffers 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO); // Bind Vertex Array Object
	
	// Bind the buffer to a vertex array.
	// OpenGL allows us to bind to several buffers at once as long as they have a different buffer type.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Copy the data to the bound buffer
	// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW: the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ARG 0: layout: (location = 0).
	// ARG 1: specifies the size of the vertex attribute
	// ARG 2: specifies the type of the data which is GL_FLOAT(a vec* in GLSL consists of floating point values).
	// ARG 3: specifies if we want the data to be normalized.
	// ARG 4: known as the stride and tells us the space between consecutive vertex attribute sets.
	// Note that since we know that the array is tightly packed(there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). 
	// ARG 5: The last parameter is of type GLvoid* and thus requires that weird cast.This is the offset of where the position data begins in the buffer.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color

	glEnableVertexAttribArray(0); // (location = 0)
	glEnableVertexAttribArray(1); // (location = 1)
	
	glBindVertexArray(0); // Unbind the VAO

	/* ---------------------------------- RENDER LOOP -----------------------------------*/

	while (!glfwWindowShouldClose(window))
	{
		// Checks if any events are triggerd (keyboard, mouse etc)
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Window color
		glClear(GL_COLOR_BUFFER_BIT);
		
		GLfloat offset = 0.5f;
		glUniform1f(glGetUniformLocation(ourShader.Program, "Offset"), offset);

		glBindVertexArray(VAO);
		ourShader.Use();

		// without EBO
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_LINE_LOOP, 0, 3);

		// with EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		
		glfwSwapBuffers(window); // Swap the screen buffers
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << "Key pressed: " << key << "\n";
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}