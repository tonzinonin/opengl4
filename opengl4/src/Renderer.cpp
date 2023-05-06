#include "Renderer.h"
#include <iostream>//处理类抽象

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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader , const float& timeValue) const
{
	shader.Bind(); 
	float dv1 = sin(timeValue) / 2.0f + 0.5f;
	float dv2 = sin(timeValue + 1.57) / 2.0f + 0.5f;
	float dv3 = sin(timeValue + 3.14) / 2.0f + 0.5f;
	std::cout << dv1 << std::endl;
	shader.SetUniform4f("dy", dv1, dv2, dv3, 1.0f);

	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}