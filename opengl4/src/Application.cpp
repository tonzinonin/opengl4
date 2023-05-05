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

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO: Error handing
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		//在栈上分配内存
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{//将着色器的源码作为字符串传入
	unsigned int program = glCreateProgram();//创建一个程序
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);//这个过程很像 C++编译中将两个不同的文件链接在一起，以便可以同时使用它们：

	glDeleteShader(vs);
	glDeleteShader(fs);//连接程序完成，可以删除shader

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//+
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//+
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//+

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;


	float positions[] = {
		-0.5f,-0.5, 0. , 0 , 1.0 , 1.0 , //0
		 0.5f, 0.5, 0. , 1.0 , 1.0 , 0 ,//1
		-0.5f, 0.5, 0. , 1.0 , 0 , 1.0 ,//2
		 0.5f,-0.5, 0. , 0 , 1.0 , 0 //3
	};

	unsigned int indices[] = {
		0 , 1 , 2,
		1 , 3 , 0
	};

	{
	
	VertexArray va;
	VertexBuffer vb(positions, 4 * 6 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(vb , layout);
	layout.Push<float>(3);
	va.AddBuffer(vb , layout);

	IndexBuffer ib(indices, 6);

	ShaderProgramSource source = ParseShader("res/Shader.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "dy"));
	ASSERT(location != -1);
	GLCall(glUniform3f(location, 0.2f, 0.3f, 0.8f));

	float r = 0.0f;
	float increment = 0.05f;

	GLCall(glBindVertexArray(0));//+
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	//GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));//线框模式

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));
		float timeValue = glfwGetTime();
		float dv1 = sin(timeValue) / 2.0f + 0.5f;
		float dv2 = cos(timeValue) / 2.0f + 0.5f;
		float dv3 = -sin(timeValue) / 2.0f + 0.5f;
		std::cout << dv1 << std::endl;
		int vertexColorLocation = glGetUniformLocation(shader, "dy");
		glUniform3f(vertexColorLocation, dv1 , dv2 , dv3);

		va.Bind();
		ib.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));
	}
	glfwTerminate();
	return 0;
}