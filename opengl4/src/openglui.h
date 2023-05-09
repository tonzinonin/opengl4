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
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
public:
	OpenglImgui(GLFWwindow* window)
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	void Draw(unsigned int IMGUI_WIDTH , unsigned int IMGUI_HEIGHT, Shader& shader , glm::vec3 &translate , float& SpecularSt , glm::vec3* cubePositions , unsigned int rendererNumber, Camera& camera)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame(); 
		ImGui::NewFrame();
		{
			ImGui::Begin("ImGui"); 
			//ImGui::SetNextItemOpen(false, ImGuiCond_Once);

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

			ImGui::SliderFloat3("Move Light Position ", &translate[0], -1.0, 1.0, "%.3f");
			ImGui::SliderFloat("Specular Strength : ", &SpecularSt, 0.1, 1.0, "%.3f"); // 创建垂直滚动条控件
			shader.Bind();
			shader.SetUniform1f("specularStrength", SpecularSt);
			shader.Unbind();

			ImGui::Text("world space:%.3f &.3f %.3f", cubePositions[0].x, cubePositions[0].y, cubePositions[0].z);
			ImGui::Text("LastX(mouse on screen) = %.3f", camera.lastY);
			ImGui::Text("Lasty(mouse on screen) = %.3f", camera.lastX);
			ImGui::Text("Yaw = %.3f", camera.yaw);
			ImGui::Text("Pitch = %.3f", camera.pitch);
			ImGui::Text("Fov = %.3f", camera.fov);
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