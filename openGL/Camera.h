#ifndef OPENGL_CAMERA_INCLUDED
#define OPENGL_CAMERA_INCLUDED

#include "Common.h"
#include "Window.h"
#include "Interfaces.h"
#include "Entity.h"

namespace openGL
{
	struct Camera : IUpdatable, Entity
	{
		/// Getters
		float* ViewMatrixPtr();
		float* ProjectionMatrixPtr();
		const glm::mat4& ViewMatrix() const;
		const glm::mat4& ProjectionMatrix() const;
		glm::mat4 ViewProjectionMatrix() const;
		/// -------

		explicit Camera(Window& window);

		void UpdateViewMatrix();

		void UpdateProjectionMatrix();

		void UpdateDirection();

		void Update() override;

		void Rotate(float dx, float dy);

		void OnMouseMove(double x, double y);

		void OnKeyPress(int key, int action);

		void Reset();

		void Initialize() override;

		virtual ~Camera();

	protected:
		openGL::Window& _window;

		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;

		float _lastX, _lastY;
		bool _firstMouseEvent = true;

	private:
		int GetKey(int key) const;
	};
}

#endif // !OPENGL_CAMERA_INCLUDED
