#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;

int main()
{
	/* ----------------------------------- INIT WINDOW -----------------------------------*/
	glfwInit();
	// Only OpenGL 3 is used
	// ENUM: GLFW_CONTEXT_VERSION_MAJOR, VALUE: 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Errors if legacy code is used
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Not resizeble by the user
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Set callback function for input
	glfwSetKeyCallback(window, key_callback);

	// etting glewExperimental to true ensures GLEW uses more modern techniques for managing OpenGL functionality. 
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
	// Build and compile our shader program
	Shader ourShader("Shaders/shader.vert", "Shaders/shader.frag");


	GLfloat vertices[] = {
		// Positions         // Colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // Bottom Left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // Top 
	};


	/* ----------------------------------- BUFFERS -----------------------------------*/
	// Vertex Buffer Object, Vertex Array Object, Element Buffer Objects
	GLuint VBO, VAO;
	// Creates a ID for the buffer 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. Bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Copy our vertices array in a buffer for OpenGL to use
	// Bind the buffer to a vertex array.
	// OpenGL allows us to bind to several buffers at once as long as they have a different buffer type.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the data to the bound buffer
	// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW: the data will change every time it is drawn.
	// 3. Then set our vertex attributes pointers
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ARG 0: specifies which vertex attribute we want to configure, layout(location = 0).
	// ARG 1: specifies the size of the vertex attribute.The vertex attribute is a vec3 so it is composed of 3 values.
	// ARG 2: specifies the type of the data which is GL_FLOAT(a vec* in GLSL consists of floating point values).
	// ARG 3: specifies if we want the data to be normalized. If we set this to GL_TRUE all the data that has a value not between 0 (or -1 for signed data) and 1 will be mapped to those values.
	// ARG 4: known as the stride and tells us the space between consecutive vertex attribute sets.Since the next set of position data is located exactly 3 times the size of a GLfloat away we specify that value as the stride.
	// Note that since we know that the array is tightly packed(there is no space between the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). 
	// Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
	// ARG 5: The last parameter is of type GLvoid* and thus requires that weird cast.This is the offset of where the position data begins in the buffer.
	// Since the position data is at the start of the data array this value is just 0. We will explore this parameter in more detail later on
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//4. Unbind the VAO
	glBindVertexArray(0);

	/* ---------------------------------- RENDER LOOP -----------------------------------*/
	while (!glfwWindowShouldClose(window))
	{
		// Checks if any events are triggerd (keyboard, mouse...)
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		GLfloat offset = 0.5f;
		glUniform1f(glGetUniformLocation(ourShader.Program, "Offset"), offset);

		glBindVertexArray(VAO);
		ourShader.Use();

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_LINE_LOOP, 0, 3);

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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