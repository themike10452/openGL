#define MOVEMENT_STEP 0.05f

#include <Windows.h>
#include <iostream>
#include "Game.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "glm\gtc\type_ptr.hpp"

namespace openGL
{
	openGL::Program program;
	openGL::Light ambientLight(glm::vec3(0.5f, 0.5f, 0.5f));
	openGL::Light light0(glm::vec3(0.0f, 0.0f, 0.8f), glm::vec3(-3.0f, 2.0f, 3.0f));
	openGL::Light light1(glm::vec3(0.8f, 0.0f, 0.0f), glm::vec3(3.0f, 2.0f, 3.0f));

	glm::mat4 model_matrix;

	void Game::OnLoad()
	{
		std::vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition("Shaders\\VertexShader.glsl", GL_VERTEX_SHADER));
		shaders.push_back(ShaderDefinition("Shaders\\FragmentShader.glsl", GL_FRAGMENT_SHADER));
		program.Compile(shaders);

		this->LoadModel(new openGL::Model(&program, "Models\\Cube.stl"));
	}

	void Game::OnUpdate()
	{
		model_matrix = glm::rotate(glm::mat4(), glm::radians((float)(Clock().ElapsedTime() * 100)), glm::vec3(0, 1, 0));
	}

	void Game::OnDraw()
	{
		glDisable(GL_CULL_FACE);

		//pass uniform data
		program.SetUniformMat4fv(VertexUniformLocations::MODEL_MATRIX, glm::value_ptr(model_matrix));
		program.SetUniformMat4fv(VertexUniformLocations::VIEW_MATRIX, _camera.ViewMatrixPtr());
		program.SetUniformMat4fv(VertexUniformLocations::PROJECTION_MATRIX, _camera.ProjectionMatrixPtr());

		program.SetUniform3fv("AmbientLight.Color", glm::value_ptr(ambientLight.Color()));

		program.SetUniform3fv("Lights[0].Color", glm::value_ptr(light0.Color()));
		program.SetUniform3fv("Lights[0].Position", glm::value_ptr(light0.Position()));
		program.SetUniform1f("Lights[0].Attenuation", light0.Attenuation());

		program.SetUniform3fv("Lights[1].Color", glm::value_ptr(light1.Color()));
		program.SetUniform3fv("Lights[1].Position", glm::value_ptr(light1.Position()));
		program.SetUniform1f("Lights[1].Attenuation", light1.Attenuation());

		this->DrawModels();
	}

	void Game::OnKeyDown(int key, int action)
	{
		Camera().OnKeyPress(key, action);

		if (action)
		{
			//if (key == GLFW_KEY_KP_SUBTRACT) { ambient_light = glm::clamp(ambient_light - glm::vec3(0.1f, 0.1f, 0.1f), 0.0f, 1.0f); }
			//if (key == GLFW_KEY_KP_ADD) { ambient_light = glm::clamp(ambient_light + glm::vec3(0.1f, 0.1f, 0.1f), 0.0f, 1.0f); }
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