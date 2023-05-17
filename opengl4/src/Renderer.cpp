#pragma once

#include "Renderer.h"
#include <iostream>//处理类抽象

void Renderer::Clear() const
{
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::MVPTrans(const unsigned int SCREEN_WIDTH, const unsigned int SCREEN_HEIGHT, const Shader& shader) const
{
	shader.Bind();
	//glm::mat4 diy = 
	//	glm::mat4(
	//	glm::vec4(1, 0, 0, 0),
	//	glm::vec4(0, 1, 0, 0),
	//	glm::vec4(0, 0, 1, 0),
	//	glm::vec4(0, 0, 0, 1.0)
	//);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.fov), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);

	ui.projectionupdate(projection);
	ui.viewupdate(view);

	shader.SetUniformMat4f("projection", projection);
	shader.SetUniformMat4f("view", view);

}
void Renderer::Mix(const int& isAddColor, const float& input, const Shader& shader ) const
{
	shader.Bind();
	if (isAddColor)
	{
		float timevalue = glfwGetTime();
		float dv1 = sin(timevalue) / 2.0f + 0.5f;
		float dv2 = sin(timevalue + 1.57) / 2.0f + 0.5f;
		float dv3 = sin(timevalue + 3.14) / 2.0f + 0.5f;
		shader.SetUniform1i("isColor", 1);
		shader.SetUniform4f("u_Color", dv1, dv2, dv3, 1.0f);
	}
	shader.SetUniform1f("mixval", input);
	shader.Unbind();
}

void Renderer::BindTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	tx.Bind(0);
}

void Renderer::Draw(const Shader& shader, const Texture& texture0, const Texture& texture1 , IndexBuffer& id) const
{
	shader.Bind();
	va.Bind();
	id.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, id.GetCount(), GL_UNSIGNED_INT, nullptr));	
	
	//shader.Unbind();
	va.Unbind();
	id.Unbind();
}
void Renderer::Cube(const Shader& shader, const glm::vec3* cubePositions ,const unsigned int& count) const
{
	va.Bind();
	shader.Bind();

	for (unsigned int i = 0; i < count; i++)
	{
		glm::mat4 model = glm::mat4(1.0);
		if (isMove)
		{
			model = glm::translate(model, ui.trans);
		}
		model = glm::translate(model, cubePositions[i]);
		float angle = i * 20.0;
		model = glm::rotate(model, glm::radians(float(glfwGetTime()) * 30 + angle), glm::vec3(1.0f, 0.3f, 0.5f));

		ui.modelupdate(model);

		shader.SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	//shader.Unbind();
	va.Unbind();
}