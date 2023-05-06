#include <iostream>
//#include "GLAD/glad.h"
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
#include "Texture.h"
#include "Shader.h"

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a Windowed mode and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!window)
	{
		GLCall(glfwTerminate())
			return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;

	constexpr float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f,	// 0
		 0.5f, -0.5f, 1.0f, 0.0f,	// 1
		 0.5f,  0.5f, 1.0f, 1.0f,	// 2
		-0.5f,  0.5f, 0.0f, 1.0f	// 3
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND))
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao))
	GLCall(glBindVertexArray(vao))

	VertexArray va;
	const VertexBuffer vb(positions, static_cast<unsigned long long>(4) * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	const IndexBuffer ib(indices, 6);

	Shader shader("res/shader/Shader.shader");
	shader.Bind();
	//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	const Texture texture("res/textures/container.jpg");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);


	va.Unbind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	Renderer renderer;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		shader.Bind();
		float timevalue = glfwGetTime();
		renderer.Draw(va, ib, shader , timevalue);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}