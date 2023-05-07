#pragma once

#include<iostream>
#include<fstream>
#include<sstream>

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath) : m_FilePath(filepath) , m_RendererID(0)
{
	const ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int v0) const
{
	GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string & name, float value) const
{
	GLCall(glUniform1f(GetUniformLocation(name), value))
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
	auto it = m_UniformLocationCache.find(name);
	if (it != m_UniformLocationCache.end())
		return it -> second;

	GLCall(const int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning:uniform '" << name << "' doesn't exist!" << std::endl;
	}
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) const
{
	std::ifstream stream(filepath);

	enum class Shadertype
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	Shadertype type = Shadertype::NONE;

	while (getline(stream, line))//一行一行地浏览文件
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{//set mode to vertex
				type = Shadertype::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{//set mode to fragment
				type = Shadertype::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str() , ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
