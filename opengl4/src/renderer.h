#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#define  ASSERT(x) if (!(x))   __debugbreak();

#define  GLCall(x) GLClearError();  x;  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class IndexBuffer;
class Shader;
class Texture;

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader , const Texture& texture0, const Texture& texture1, const int& isAddColor , const float& input) const;
};