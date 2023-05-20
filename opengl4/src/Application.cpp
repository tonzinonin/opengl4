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
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "ReadVertex.h"
#include "openglui.h"	
#include "Model.h"

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
float lightboxposition[ARR_SIZE];

Camera camera;

unsigned int LoadCubeTexture(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void LoadBasicShader(glm::vec3 *lightPositions, Shader& shader)
{
	shader.Bind();
	shader.SetUniform1f("shininess", 32.0f);
	for (int i = 0; i < 1; i++)
	{
		std::string str = "pointlight[";
		char idx = i + '0';
		str.push_back(idx);
		std::string str1 = str + "].position";
		shader.SetUniformVec3(str1.c_str(), lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);

		std::string str2 = str + "].diffuse";
		shader.SetUniformVec3(str2.c_str(), 0.5f, 0.5f, 0.5f);

		std::string str3 = str + "].ambient";
		shader.SetUniformVec3(str3.c_str(), 0.2f, 0.2f, 0.2f);

		std::string str4 = str + "].specular";
		shader.SetUniformVec3(str4.c_str(), 1.0f, 1.0f, 1.0f);

		std::string str5 = str + "].quadratic";
		shader.SetUniform1f(str5.c_str(), 0.032f);

		std::string str6 = str + "].constant";
		shader.SetUniform1f(str6.c_str(), 1.0f);

		std::string str7 = str + "].linea";
		shader.SetUniform1f(str7.c_str(), 0.09f);
	}
	shader.SetUniformVec3("dirlight.direction", -0.2f, -1.0f, -0.3f);

	shader.SetUniformVec3("dirlight.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniformVec3("dirlight.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	shader.SetUniformVec3("dirlight.specular", 1.0f, 1.0f, 1.0f);

	shader.SetUniformVec3("spotlight.ambient", 0.2f, 0.2f, 0.2f);
	shader.SetUniformVec3("spotlight.diffuse", 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
	shader.SetUniformVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);

	shader.SetUniform1f("spotlight.constant", 1.0f);
	shader.SetUniform1f("spotlight.linea", 0.09f);
	shader.SetUniform1f("spotlight.quadratic", 0.032f);

	shader.SetUniform1f("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.SetUniform1f("spotlight.outerCutOff", glm::cos(glm::radians(15.5f)));


	shader.Unbind();
}

int main(void)
{
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "cube", nullptr, nullptr);
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

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//GLCall(glEnable(GL_BLEND))
	//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))//开启混合
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	stbi_set_flip_vertically_on_load(false);

	VertexUnion vu("res/vertex/cubeVertex.txt");
	vu.value(positions);
	VertexUnion skybox("res/vertex/skybox.txt");
	skybox.value(lightboxposition);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 lightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	std::vector<std::string> faces
	{
		"res/textures/SkyBox/right.jpg",
		"res/textures/SkyBox/left.jpg",
		"res/textures/SkyBox/top.jpg",
		"res/textures/SkyBox/bottom.jpg",
		"res/textures/SkyBox/front.jpg",
		"res/textures/SkyBox/back.jpg"
	};

	//-----
	unsigned int cubemapTexture = LoadCubeTexture(faces);
	std::cout << "count : " <<  skybox.GetCount() << std::endl;
	VertexArray skyboxVAO;
	VertexBuffer skyboxvb(lightboxposition, skybox.GetCount() * sizeof(float));
	VertexBufferLayout skylayout;
	skylayout.Push<float>(3);
	skyboxVAO.AddBuffer(skyboxvb, skylayout);

	VertexArray va;
	VertexBuffer vb(positions, vu.GetCount() * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	Model ourModel("res/model/nanosuit.obj");//加载obj模型

	Shader shader("res/shader/ourShader.vert", "res/shader/ourShader.frag");
	Shader lightShader("res/shader/Light.vert", "res/shader/Light.frag");
	Shader skyShader("res/shader/skybox.vert", "res/shader/skybox.frag");

	LoadBasicShader(lightPositions , shader);//加载基础受光物体的shader

	lightShader.Bind();
	lightShader.SetUniformMat4f("scale",
		glm::mat4(
			0.3, 0, 0, 0,
			0, 0.3, 0, 0,
			0, 0, 0.3, 0,
			0, 0, 0, 1
		));
	std::cout << "lightShaderID:" << lightShader.GetRendererID() << std::endl;
	lightShader.Unbind();//加载点光源shader

	Texture texture0("res/textures/container2.png");
	Texture texture1("res/textures/container2_specular.png");
	//Texture texture2("res/textures/matrix.jpg");
	//shader.SetUniform1i("material.glow", 2);//加载cube基础贴图

	va.Unbind();
	vb.Unbind();

	OpenglImgui ui(window, shader, camera, cubePositions, 2);

	Renderer renderer(camera, ui, va, texture0);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float timeValue = glfwGetTime();
		deltaTime = timeValue - lastFrame;
		lastFrame = timeValue;

		renderer.Clear();
		//texture2.Bind(2);

		glDepthMask(GL_FALSE);
		skyShader.Bind();
		skyboxVAO.Bind();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);
		//skyShader.SetUniformMat4f("projection", projection);
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyShader.GetRendererID(), "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyShader.GetRendererID(), "projection"), 1, GL_FALSE, &projection[0][0]);

		//shader.SetUniformMat4f("view", camera.GetViewMatrix());
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		renderer.MVPTrans(SCREEN_WIDTH, SCREEN_WIDTH, shader);
		renderer.MVPTrans(SCREEN_WIDTH, SCREEN_WIDTH, lightShader);
		renderer.Cube(lightShader, lightPositions, 2);
		skyShader.Unbind();
		skyboxVAO.Unbind();

		shader.Bind();
		glUniform3f(glGetUniformLocation(shader.GetRendererID(), "spotlight.direction"), camera.cameraFront.x, camera.cameraFront.y, camera.cameraFront.z);
		glUniform3f(glGetUniformLocation(shader.GetRendererID(), "viewPos"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
		glUniform3f(glGetUniformLocation(shader.GetRendererID(), "spotlight.position"), camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

		ourModel.Draw(shader);
		texture0.Bind(0);
		texture1.Bind(1);
		//texture2.Bind(2);
		renderer.Cube(shader, cubePositions, 10);

		//shader.Unbind();
		//ourShader.Unbind();

		//ui.Draw(IMGUI_WIDTH, IMGUI_HEIGHT);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and events */
		glfwPollEvents();
	}

	//ui.~OpenglImgui();

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) { camera.LookMovement(xpos, ypos); }

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { camera.ScrollMovement(xoffset, yoffset); }

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