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
	static glm::vec4 light0_position(-2.0f, 2.0f, 3.0f, 1.0f);

	openGL::Program program;
	glm::mat4 model_matrix;

	void Game::OnLoad()
	{
		std::vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition("Shaders\\VertexShader.glsl", GL_VERTEX_SHADER));
		shaders.push_back(ShaderDefinition("Shaders\\FragmentShader.glsl", GL_FRAGMENT_SHADER));
		program.Compile(shaders);

		this->LoadModel(new openGL::Model(&program, "Models\\Nefertiti.stl"));
	}

	void Game::OnUpdate()
	{
	}

	void Game::OnDraw()
	{
		glDisable(GL_CULL_FACE);

		//pass uniform data
		program.SetUniformMat4fv(VertexUniformLocations::MODEL_MATRIX, glm::value_ptr(model_matrix));
		program.SetUniformMat4fv(VertexUniformLocations::VIEW_MATRIX, _camera.ViewMatrixPtr());
		program.SetUniformMat4fv(VertexUniformLocations::PROJECTION_MATRIX, _camera.ProjectionMatrixPtr());
		program.SetUniform4fv(VertexUniformLocations::AMBIENT_LIGHT, glm::value_ptr(ambient_light));
		program.SetUniform4fv(VertexUniformLocations::LIGHT0_POSITION, glm::value_ptr(light0_position));

		this->DrawModels();
	}

	void Game::OnKeyDown(int key, int action)
	{
		Camera().OnKeyPress(key, action);

		if (action)
		{
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