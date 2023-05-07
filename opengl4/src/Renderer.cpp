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
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::MVPTrans(const unsigned int width, const unsigned int height, const Shader& shader) const
{
	shader.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);

	glm::mat4 result = glm::mat4(1.0f);
	result = projection * view * model;

	shader.SetUniformMat4f("transform", result);

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
		std::cout << dv1 << std::endl;
		shader.SetUniform1i("isColor", 1);
		shader.SetUniform4f("u_Color", dv1, dv2, dv3, 1.0f);
	}
	shader.SetUniform1f("mixval", input);
	shader.Unbind();
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader , const Texture& texture0, const Texture& texture1) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glActiveTexture(GL_TEXTURE0);
	texture0.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	texture1.Bind(1);

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}