#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"

class OpenglImgui
{
public:
	OpenglImgui(GLFWwindow* window)
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui::StyleColorsDark();
		const char* glsl_version = "#version 330";
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	void Darw(unsigned int IMGUI_WIDTH,unsigned int IMGUI_HEIGHT, Camera& camera)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame(); 
		ImGui::NewFrame();
		{
			ImGui::Begin("ImGui");
			ImGui::Text("LastX = %.3f", camera.lastY);
			ImGui::Text("Lasty = %.3f", camera.lastX);
			ImGui::Text("Yaw = %.3f", camera.yaw);
			ImGui::Text("Pitch = %.3f", camera.pitch);
			ImGui::Text("Fov = %.3f", camera.fov);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Press R to enable/unenable cursor in window");
			ImGui::SetWindowSize(ImVec2(IMGUI_WIDTH, IMGUI_HEIGHT));
			ImGui::End();
		}
		//// ´´½¨ GUI ÔªËØ
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	~OpenglImgui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};