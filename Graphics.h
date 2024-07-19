#ifndef Graphics_H
#define Graphics_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string_view>
#include<vector>
#include"Shader.h"

using std::vector;

struct Colors
{
	glm::vec3 Amber{ glm::vec3(1.0f, 0.75f, 0.0f) };
	glm::vec3 White{ glm::vec3(1.0f, 1.0f, 1.0f) };
	glm::vec3 MayiBleu{ 0.0f, 0.102f, 1.0f };
};

class GraphicalObj {

public:
	GraphicalObj();
	~GraphicalObj();
	void ShaderSetup(Shader& shader, const char* TexturefilePath);
	void VertexUpdate(vector<float>* vertices, vector<int>* indices);
	void BufferUpdate();
	void DrawShape(glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f));
	void transform(glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f), glm::vec3 translate = glm::vec3(0.0f,0.0f,0.0f), glm::float32 rotate = 0.0f);
	Shader getShader();

	vector<float> vertexBuffer = {
		// Postition             Colors                   Texture Coords
		0.5f, 0.5f, 0.0f,        0.0f, 0.0f, 0.0f,        1.0f, 1.0f,          //top right
		0.5f, -0.5f, 0.0f,       0.0f, 0.0f, 0.0f,        1.0f, 0.0f,          //bottom right
		-0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 0.0f,        0.0f, 0.0f,          //bottom left
		-0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 0.0f,      0.0f, 1.0f           //top left
	};

	vector<int> indexBuffer = {
		0, 1, 3,
		1, 2, 3,
	};

	glm::mat4 model = glm::mat4(1.0f);

private:
	Shader Objshader;
	GLuint VBO{}, VAO{}, EBO{};
	const char* texturePath;
};
#endif // !GUI_H4