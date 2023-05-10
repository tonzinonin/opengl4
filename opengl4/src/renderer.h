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
	Camera& camera;
	OpenglImgui& ui;
	VertexArray& va;
	Texture& tx;
public:
	bool isMove;

	Renderer(Camera& cam, OpenglImgui& ui, VertexArray& va , Texture& texture)
	:camera(cam), ui(ui), va(va) , tx(texture){}
	void Clear() const;
	void MVPTrans(const unsigned int width , const unsigned int height , const Shader& shader) const;

	void BindTexture() const;

	void Mix(const int& isAddColor, const float& input, const Shader& shader) const;
	void Draw(const Shader& shader, const Texture& texture0, const Texture& texture1 , IndexBuffer& id) const;
	void Cube(const Shader& shader, const glm::vec3* cubePositions ,const unsigned int& count) const;

	inline void UpdateViewPos(Shader& shader) { shader.Bind();  shader.SetUniformVec3("viewPos", camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z); shader.Unbind(); }
};