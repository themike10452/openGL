#ifndef OPENGL_GAME_INCLUDED
#define OPENGL_GAME_INCLUDED

#include <GL\glew.h>
#include "Program.h"
#include "Window.h"
#include "Camera.h"
#include "Clock.h"
#include "Model.h"

namespace openGL
{
	struct Game
	{
		openGL::Camera& Camera() { return _camera; }
		openGL::Clock& Clock() { return _clock; }
		openGL::Window& Window() { return _window; }

		Game()
			: _camera(_window)
		{
		}

		void Initialize()
		{
			if (!glfwInit()) throw std::exception("glfwinit() failed");
			_window.Create(this);

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK) throw std::exception("glewInit() failed");

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			AddComponent(&_camera);
			AddComponent(&_clock);
			AddComponent(&_window);

			OnLoad();
			InitializeComponents();
		}

		void Start()
		{
			_clock.Start();

			while (!_window.ShouldClose())
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				OnUpdate();
				UpdateComponents();
				OnDraw();

				_window.SwapBuffers();
				glfwPollEvents();
			}

			Stop();
		}

		void Stop()
		{
			_clock.Stop();
		}

		void Exit()
		{
			Window().Close();
		}

		void LoadModel(openGL::Model* model)
		{
			_models.push_back(model);
		}

		void DrawModels()
		{
			for (openGL::Model* model : _models)
			{
				model->Draw();
			}
		}

		int GetKey(int key) const
		{
			return _window.GetKey(key);
		}

		void AddComponent(IComponent* component)
		{
			_components.push_back(component);
		}

		virtual void OnLoad();
		virtual void OnUpdate();
		virtual void OnDraw();
		virtual void OnKeyDown(int key, int action);
		virtual void OnMouseDown(int button, int action);
		virtual void OnMouseMove(double x, double y);

		virtual ~Game()
		{
			glfwTerminate();
		}

	protected:
		openGL::Camera _camera;
		openGL::Clock _clock;
		openGL::Window _window;

		std::vector<IComponent*> _components;
		std::vector<openGL::Model*> _models;

		void InitializeComponents()
		{
			for (IComponent* c : _components)
			{
				c->Initialize();
			}
		}

		void UpdateComponents()
		{
			for(IComponent* c : _components)
			{
				if (IUpdatable* u = dynamic_cast<IUpdatable*>(c))
				{
					u->Update();
				}
			}
		}
	};
}

#endif // !OPENGL_GAME_INCLUDED

