#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//+
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//+
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//+

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	float positions[] = {
		-0.5f,-0.5, 0. , 0.75 , 0 , 0 , //0
		 0.5f, 0.5, 0. , 0.25 , 0.5 , 0 ,//1
		-0.5f, 0.5, 0. , 0 , 0.5 , 0.25 ,//2
		 0.5f,-0.5, 0. , 0 , 0 , 0.75 //3
	};

	unsigned int indices[] = {
		0 , 1 , 2,
		1 , 3 , 0
	};

	{
	
	VertexArray va;
	VertexBuffer vb(positions, 4 * 6 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(vb , layout);
	layout.Push<float>(3);
	va.AddBuffer(vb , layout);

	IndexBuffer ib(indices, 6);
	
	Shader shader("res/Shader.shader");
	shader.Bind();
	shader.SetUniform4f("dy", 0.8f, 0.3f, 0.8f, 1.0f);

	Renderer renderer;

	float r = 0.0f;
	float increment = 0.05f;

	va.Bind();
	shader.Unbind();
	vb.Unbind();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		renderer.Clear();

		shader.Bind();

		renderer.Draw(va, ib, shader, static_cast<float>(glfwGetTime()));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	}
	glfwTerminate();
	return 0;
}