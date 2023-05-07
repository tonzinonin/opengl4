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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader , const Texture& texture0, const Texture& texture1 ,
	const int& isAddColor , const float& input) const
{
	shader.Bind(); 

	glActiveTexture(GL_TEXTURE0);
	texture0.Bind(0);
	glActiveTexture(GL_TEXTURE1);
	texture1.Bind(1);
	
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(0.75f, 1.0f, 1.0f));
	trans = glm::rotate(trans, float(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

	shader.SetUniformMat4f("transform", trans);

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

	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}