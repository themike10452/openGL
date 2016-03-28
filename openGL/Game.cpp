#define MOVEMENT_STEP 0.05f

#include <Windows.h>
#include <iostream>
#include "Game.h"
#include "Camera.h"
#include "Model.h"
#include "glm\gtc\type_ptr.hpp"

namespace openGL
{
	static glm::vec4 ambient_light(1.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 model_matrix;

	void Game::OnLoad()
	{
		std::vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition("Shaders\\VertexShader.glsl", GL_VERTEX_SHADER));
		shaders.push_back(ShaderDefinition("Shaders\\FragmentShader.glsl", GL_FRAGMENT_SHADER));
		this->CompileShader(shaders);

		openGL::Model cube = openGL::Model("Models\\Cube.obj");
		this->LoadModel(cube);
	}

	void Game::OnUpdate()
	{
	}

	void Game::OnDraw()
	{
		this->UseProgram();
		glDisable(GL_CULL_FACE);

		//pass uniform data
		glUniformMatrix4fv(VertexUniformLocations::MODEL_MATRIX, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(VertexUniformLocations::VIEW_MATRIX, 1, GL_FALSE, _camera.ViewMatrixPtr());
		glUniformMatrix4fv(VertexUniformLocations::PROJECTION_MATRIX, 1, GL_FALSE, _camera.ProjectionMatrixPtr());
		glUniform4fv(VertexUniformLocations::AMBIENT_LIGHT, 1, &ambient_light[0]);

		this->DrawModels();
	}

	void Game::OnKeyDown(int key, int action)
	{
		Camera().OnKeyPress(key, action);

		if (action) {
			if (key == GLFW_KEY_KP_SUBTRACT) { ambient_light = glm::clamp(ambient_light - glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), 0.0f, 1.0f); }
			if (key == GLFW_KEY_KP_ADD) { ambient_light = glm::clamp(ambient_light + glm::vec4(0.1f, 0.1f, 0.1f, 0.0f), 0.0f, 1.0f); }
			if (key == GLFW_KEY_ESCAPE) { Exit(); }
		}
	}

	void Game::OnMouseDown(int button, int action)
	{
	}

	void Game::OnMouseMove(double x, double y)
	{
		Camera().OnMouseMove(x, y);
	}
}

void main()
{
	try
	{
		openGL::Game game;
		game.Initialize();
		game.Start();
		exit(EXIT_SUCCESS);
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, e.what(), "Exception", MB_OK);
		exit(EXIT_FAILURE);
	}
}