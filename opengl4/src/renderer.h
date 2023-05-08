#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#define  ASSERT(x) if (!(x))   __debugbreak();

#define  GLCall(x) GLClearError();  x;  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class IndexBuffer;
class Shader;
class Texture;
class Camera;

class Renderer
{
public:
	void Clear() const;
	void MVPTrans(const unsigned int width , const unsigned int height, const Shader& shader, Camera& camera) const;
	void Mix(const int& isAddColor, const float& input, const Shader& shader) const;
	void DrawCube(const VertexArray& va, const Shader& shader, const Texture& texture0, const Texture& texture1 , const glm::vec3 *cubePositions) const;
	void Draw(const VertexArray& va, const Shader& shader, const Texture& texture0, const Texture& texture1, const IndexBuffer& id) const;
};