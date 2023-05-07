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

#include "stb_image/stb_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

void processInput(GLFWwindow* window);

float mixValue = 0.2;

#define SCREEN_WIDTH 640
#define SCREEM_HEIGHT 480

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a Windowed mode and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640,480, "Hello World", nullptr, nullptr);
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
	     0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   -1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   -1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // 左上
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//GLCall(glEnable(GL_BLEND))
	//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))//开启混合

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao))
	GLCall(glBindVertexArray(vao))

	VertexArray va;
	const VertexBuffer vb(positions,sizeof(positions));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	const IndexBuffer ib(indices, 6);

	Shader shader("res/shader/Shader.shader");

	const Texture texture0("res/textures/container.jpg");
	texture0.Bind();
	const Texture texture1("res/textures/awesomeface.png");
	texture1.Bind();

	shader.Bind();

	shader.SetUniform1i("texture0", 0);
	shader.SetUniform1i("texture1", 1);

	va.Unbind();
	shader.Unbind();
	vb.Unbind();
	ib.Unbind();

	Renderer renderer;

	//ImGui::CreateContext();
	//ImGui_ImplGlfw_InitForOpenGL(window, true);
	//ImGui::StyleColorsDark();

	//const char* glsl_version = "#version 330";
	//ImGui_ImplOpenGL3_Init(glsl_version);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		/* Render here */
		renderer.Clear();

		renderer.MVPTrans(SCREEN_WIDTH, SCREEM_HEIGHT, shader);
		renderer.Mix(false, mixValue, shader);
		renderer.Draw(va, ib, shader, texture0 , texture1);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and events */
		glfwPollEvents();
	}

	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.01;
		if (mixValue >= 1.00) mixValue = 1.00;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.01;
		if (mixValue <= 0.00) mixValue = 0.00;
	}
}