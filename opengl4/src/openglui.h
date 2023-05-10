#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Shader.h"

class OpenglImgui
{
private:
	Shader& shader;
	Camera& camera;
	glm::vec3* cubePositions;
	unsigned int rendererNumber;

public:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 trans= glm::vec3(0., 0., 0.);

	float material_shininess = 32.0f;
	glm::vec3 light_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);


	OpenglImgui(GLFWwindow* window , Shader& shader , Camera& camera , glm::vec3* cubePositions , unsigned int rendererNumber) :
		shader(shader) , camera(camera) , cubePositions(cubePositions) , rendererNumber(rendererNumber)
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	void ShowLightInformation(glm::vec3 lightCube)
	{
		ImGui::SliderFloat3("Move Light Position ", &trans[0], -1.0, 1.0, "%.3f");

		ImGui::SliderFloat("material.shininess", &material_shininess, 0.1, 64.0, "%.3f");
		ImGui::SliderFloat3("light.ambient", &light_ambient[0], 0.0, 1.0, "%.3f");
		ImGui::SliderFloat3("light.diffuse", &light_diffuse[0], 0.0, 1.0, "%.3f"); // 将光照调暗了一些以搭配场景
		ImGui::SliderFloat3("light.specular", &light_specular[0], 0.0, 1.0, "%.3f");

		shader.Bind();
		shader.SetUniform1f("material.shininess", material_shininess);
		shader.SetUniformVec3("light.ambient", light_ambient.x , light_ambient.y , light_ambient.z);
		shader.SetUniformVec3("light.diffuse", light_diffuse.x , light_diffuse.y , light_diffuse.z); // 将光照调暗了一些以搭配场景
		shader.SetUniformVec3("light.specular", light_specular.x , light_specular.y , light_specular.z);
		shader.Unbind();
		//ImGui::SliderFloat(light)
	}
	void ShowPositionInformation()
	{
		if (ImGui::CollapsingHeader("world space", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (unsigned int i = 0; i < rendererNumber; i++)
			{
				glm::vec4 tempvec = model * glm::vec4(cubePositions[i], 0.0);
				ImGui::Text("object %d :%.3f %.3f %.3f", i, tempvec.x, tempvec.y, tempvec.z);
			}
		}

		if (ImGui::CollapsingHeader("view space", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (unsigned int i = 0; i < rendererNumber; i++)
			{
				glm::vec4 tempvec = view * model * glm::vec4(cubePositions[i], 0.0);
				ImGui::Text("object %d :%.3f %.3f %.3f", i, tempvec.x, tempvec.y, tempvec.z);
			}
		}

		if (ImGui::CollapsingHeader("clip space", ImGuiTreeNodeFlags_DefaultOpen)) {
			for (unsigned int i = 0; i < rendererNumber; i++)
			{
				glm::vec4 tempvec = projection * view * model * glm::vec4(cubePositions[i], 0.0);
				ImGui::Text("object %d :%.3f %.3f %.3f", i, tempvec.x, tempvec.y, tempvec.z);
			}
		}
		ImGui::Text("world space:%.3f &.3f %.3f", cubePositions[0].x, cubePositions[0].y, cubePositions[0].z);
		ImGui::Text("LastX(mouse on screen) = %.3f", camera.lastY);
		ImGui::Text("Lasty(mouse on screen) = %.3f", camera.lastX);
		ImGui::Text("Yaw = %.3f", camera.yaw);
		ImGui::Text("Pitch = %.3f", camera.pitch);
		ImGui::Text("Fov = %.3f", camera.fov);
	}
	void Draw(unsigned int IMGUI_WIDTH , unsigned int IMGUI_HEIGHT)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame(); 
		ImGui::NewFrame();
		{
			ImGui::Begin("ImGui"); 
			//ImGui::SetNextItemOpen(false, ImGuiCond_Once);


			ShowPositionInformation();
			ShowLightInformation(cubePositions[1]);
			//ImGui::SliderFloat("Specular Strength : ", &SpecularSt, 0.1, 32.0, "%.3f"); // 创建垂直滚动条控件
			//shader.Bind();
			//shader.SetUniform1f("specularStrength", SpecularSt);
			//shader.Unbind();


			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Press R to enable/unenable cursor in window");
			ImGui::SetWindowSize(ImVec2(IMGUI_WIDTH, IMGUI_HEIGHT));
			ImGui::End();
		}
		//// 创建 GUI 元素
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	~OpenglImgui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void modelupdate(glm::mat4 mat)
	{
		model = mat;
	}
	void viewupdate(glm::mat4 mat)
	{
		view = mat;
	}
	void projectionupdate(glm::mat4 mat)
	{
		projection = mat;
	}

};