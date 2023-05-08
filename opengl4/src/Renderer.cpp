#include "Renderer.h"
#include <iostream>//处理类抽象
#include <GLFW/glfw3.h>

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::MVPTrans(const unsigned int SCREEN_WIDTH , const unsigned int SCREEN_HEIGHT , const Shader& shader , Camera& camera ) const
{
	shader.Bind();

	float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera.fov), float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.1f, 100.0f);
	shader.SetUniformMat4f("projection", projection);
	shader.SetUniformMat4f("view", view);

	shader.Unbind();
}

void Renderer::Mix(const int& isAddColor, const float& input, const Shader& shader) const
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

void Renderer::DrawCube(const VertexArray& va, const Shader& shader , const Texture& texture0, const Texture& texture1 , const glm::vec3 *cubePositions) const
{
	shader.Bind();
	va.Bind();

	glActiveTexture(GL_TEXTURE0);
	texture0.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	texture1.Bind(1);

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, cubePositions[i]);	
		float angle = i * 20.0;
		model = glm::rotate(model, glm::radians(float(glfwGetTime()) * 30 + angle), glm::vec3(1.0f, 0.3f, 0.5f));
		shader.SetUniformMat4f("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, const Texture& texture0, const Texture& texture1, const IndexBuffer& id) const
{
	shader.Bind();
	va.Bind();
	id.Bind();

	glActiveTexture(GL_TEXTURE0);
	texture0.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	texture1.Bind(1);

	GLCall(glDrawElements(GL_TRIANGLES, id.GetCount(), GL_UNSIGNED_INT, nullptr));
}