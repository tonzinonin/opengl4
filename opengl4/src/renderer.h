#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "openglui.h"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{ 
private:
public:
	void Clear() const;
	void MVPTrans(const unsigned int width , const unsigned int height, const Shader& shader, Camera& camera , OpenglImgui& openglui) const;
	void Mix(const int& isAddColor, const float& input, const Shader& shader ) const;
	void DrawCube(const VertexArray& va, const Shader& shader, const Texture& texture0, const Texture& texture1 , const glm::vec3 *cubePositions, OpenglImgui& openglui, unsigned int rendererNumber) const;
	void Draw(const VertexArray& va, const Shader& shader, const Texture& texture0, const Texture& texture1, const IndexBuffer& id) const;
	void LightCube(const VertexArray& va, const Shader& shader, const glm::vec3 cubePositions, OpenglImgui& openglui, unsigned int rendererNumber) const;
};