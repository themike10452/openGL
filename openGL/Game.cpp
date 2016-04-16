#include <Windows.h>
#include <iostream>
#include "Common.h"
#include "Game.h"
#include "Camera.h"
#include "PointLight.h"
#include "Terrain.h"
#include "Player.h"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace openGL
{
	openGL::Light ambientLight(glm::vec3(0.4f));
	openGL::PointLight SunLight(glm::vec3(0.5f), glm::vec3(212.36, 268.34, 500.0), 0.0f);

	glm::mat4 model_matrix;

	std::vector<openGL::Model> models;

	openGL::Program program;
	openGL::Terrain terrain;
	//openGL::Player player("Models\\NanoSuit\\nanosuit.obj");

	void Game::OnLoad()
	{
		std::vector<ShaderDefinition> shaders;
		shaders.push_back(ShaderDefinition("Shaders\\1\\Base.vert", GL_VERTEX_SHADER));
		shaders.push_back(ShaderDefinition("Shaders\\1\\Base.frag", GL_FRAGMENT_SHADER));
		program.Compile(shaders);

		terrain.LoadHeightMap(1000, 1000, "Resources\\Terrain_HM.bmp");
		terrain.LoadTexture("Resources\\Terrain_TM.bmp");
		terrain.Initialize();

		models.push_back(openGL::Model("Models\\Earth.obj"));
	}

	void Game::OnUpdate()
	{
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);

		glm::vec3 cameraPosition = _camera.Position();

		cameraPosition.y = glm::max(terrain.GetTerrainHeight(cameraPosition) + 2, cameraPosition.y);
		_camera.SetPosition(cameraPosition);

		std::string title(std::to_string(cameraPosition.x) + " " + std::to_string(cameraPosition.z));
		this->Window().SetTitle(title.c_str());
	}

	void Game::OnDraw()
	{
		program.Bind();

		program.SetUniformMat4fv("ViewMatrix", _camera.ViewMatrixPtr());
		program.SetUniformMat4fv("ProjectionMatrix", _camera.ProjectionMatrixPtr());
		program.SetUniform3fv("ViewPos", glm::value_ptr(_camera.Position()));
		program.SetUniform3fv("SunPosition", glm::value_ptr(SunLight.Position()));
		program.SetUniform3fv("AmbientLight.Color", glm::value_ptr(ambientLight.Color()));

		//draw light source
		glm::mat4 model = glm::translate(model_matrix, SunLight.Position());
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		program.SetUniformMat4fv("ModelMatrix", glm::value_ptr(model));

		models[0].Draw(&program);
		//------------------

		program.SetUniformMat4fv("ModelMatrix", glm::value_ptr(model_matrix));

		terrain.Draw();
	}

	void Game::OnKeyDown(int key, int action)
	{
		Camera().OnKeyPress(key, action);

		if (action)
		{
			if (key == GLFW_KEY_KP_ADD) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
			if (key == GLFW_KEY_KP_SUBTRACT) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
			if (key == GLFW_KEY_ESCAPE) { Exit(); }
			if (key == GLFW_KEY_INSERT) { terrain.ToggleNormalMapping(); }

			if (key == GLFW_KEY_KP_8) { SunLight.Move(0, 0, -10); }
			if (key == GLFW_KEY_KP_2) { SunLight.Move(0, 0, 10); }
			if (key == GLFW_KEY_KP_4) { SunLight.Move(-10, 0, 0); }
			if (key == GLFW_KEY_KP_6) { SunLight.Move(10, 0, 0); }
			if (key == GLFW_KEY_KP_5) { SunLight.Move(0, -10, 0); }
			if (key == GLFW_KEY_KP_0) { SunLight.Move(0, 10, 0); }
		}
	}

	void Game::OnMouseDown(int button, int action)
	{
	}

	void Game::OnMouseMove(double x, double y)
	{
		this->Camera().OnMouseMove(x, y);
	}
}