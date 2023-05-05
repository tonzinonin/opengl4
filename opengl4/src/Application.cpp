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
		-0.5f,-0.5, 0. , 0 , 1.0 , 1.0 , //0
		 0.5f, 0.5, 0. , 1.0 , 1.0 , 0 ,//1
		-0.5f, 0.5, 0. , 1.0 , 0 , 1.0 ,//2
		 0.5f,-0.5, 0. , 0 , 1.0 , 0 //3
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
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	float r = 0.0f;
	float increment = 0.05f;

	va.Bind();
	shader.Unbind();
	vb.Unbind();

	//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));//Ïß¿òÄ£Ê½

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		shader.Bind();

		float timeValue = glfwGetTime();
		float dv1 = sin(timeValue) / 2.0f + 0.5f;
		float dv2 = cos(timeValue) / 2.0f + 0.5f;
		float dv3 = -sin(timeValue) / 2.0f + 0.5f;
		std::cout << dv1 << std::endl;
		shader.SetUniform4f("dy", dv1, dv2, dv3, 1.0f);

		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	}
	glfwTerminate();
	return 0;
}