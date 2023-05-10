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
#include "Camera.h"
#include "ReadVertex.h"
#include "openglui.h"	

#include "stb_image/stb_image.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 1080
#define IMGUI_WIDTH 400
#define IMGUI_HEIGHT 600
#define ARR_SIZE 2000

float mixValue = 0.2;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool isMouse = false;
float SpecularSt = 0.5;
float positions[ARR_SIZE];

Camera camera;

int main(void)
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT , "cube", nullptr, nullptr);
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

	VertexUnion vu("res/vertex/cubeVertex.txt");
	vu.value(positions);

	glm::vec3 cubePositions[] ={
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 lightPositions[] = {
		glm::vec3(-1.5f, -2.2f, -2.5f),
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao))
	GLCall(glBindVertexArray(vao))

	//std::cout << " ! " <<  vu.GetCount() << std::endl;
	VertexArray va;
	VertexBuffer vb(positions, vu.GetCount() * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	Shader shader("res/shader/Object.shader");
	shader.Bind();
	
	shader.SetUniform1f("material.shininess", 32.0f);

	shader.SetUniformVec3("light.position", lightPositions[0].x, lightPositions[0].y, lightPositions[0].z);
	shader.SetUniformVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniformVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	shader.SetUniformVec3("light.specular", 1.0f, 1.0f, 1.0f);

	shader.SetUniform1f("light.constant", 1.0f);
	shader.SetUniform1f("light.linea", 0.09f);
	shader.SetUniform1f("light.quadratic", 0.032f);

	shader.SetUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
	shader.SetUniform1f("light.outerCutOff", glm::cos(glm::radians(15.5f)));
	//shader.SetUniformVec3("light.direction", -0.2f, -1.0f, -0.3f);

	Texture texture0("res/textures/container2.png");
	Texture texture1("res/textures/container2_specular.png");
	//Texture texture2("res/textures/matrix.jpg");

	shader.SetUniform1i("material.diffuse", 0);
	shader.SetUniform1i("material.specular", 1);
	//shader.SetUniform1i("material.glow", 2);
	shader.Unbind();

	Shader lightShader("res/shader/Light.shader");
	lightShader.Bind();
	lightShader.SetUniformMat4f("scale",
		glm::mat4(
			0.3, 0, 0, 0,
			0, 0.3, 0, 0,
			0, 0, 0.3, 0,
			0, 0, 0, 1
		));
	lightShader.Unbind();
	va.Unbind();
	vb.Unbind();

	OpenglImgui ui(window, shader, camera, cubePositions, 9);

	Renderer renderer(camera , ui , va , texture0);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//GLCall(glEnable(GL_BLEND))
	//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))//开启混合
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;

		texture0.Bind(0);
		texture1.Bind(1);
		//texture2.Bind(2);

		renderer.Clear();
		renderer.MVPTrans(SCREEN_WIDTH, SCREEN_WIDTH, shader);
		renderer.MVPTrans(SCREEN_WIDTH, SCREEN_WIDTH, lightShader);

		shader.Bind();  
		//shader.SetUniform1f("movement", timeValue);
		shader.SetUniformVec3("light.direction", camera.cameraFront.x, camera.cameraFront.y, camera.cameraFront.z);
		shader.SetUniformVec3("viewPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		shader.SetUniformVec3("light.position", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		shader.Unbind();

		//renderer.Mix(false, mixValue, shader );
		//renderer.DrawCube(va, shader, texture0 , texture1 , cubePositions , ui , rendererNumber );
		
		renderer.Cube(shader, cubePositions ,9);
		renderer.isMove = true;
		renderer.Cube(lightShader, lightPositions ,1);
		renderer.isMove = false;

		ui.Draw(IMGUI_WIDTH , IMGUI_HEIGHT);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and events */
		glfwPollEvents();
	}

	ui.~OpenglImgui();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwDestroyWindow(window); 
		glfwTerminate();
	}
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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.PositionMovement(deltaTime, FORWARD);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.PositionMovement(deltaTime, BACKWARD);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.PositionMovement(deltaTime, RIGHT);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.PositionMovement(deltaTime, LEFT);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){ camera.LookMovement(xpos, ypos); }

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){ camera.ScrollMovement(xoffset, yoffset); }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
	{
		if (isMouse == false)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
			glfwSetCursorPosCallback(window, NULL);
			isMouse = true;
			std::cout << '!' << std::endl;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, mouse_callback);
			isMouse = false;
			std::cout << '?' << std::endl;
		}
	}
}