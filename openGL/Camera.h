#ifndef OPENGL_CAMERA_INCLUDED
#define OPENGL_CAMERA_INCLUDED

#include "Window.h"
#include "Interfaces.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

namespace openGL
{
	struct Camera : IUpdatable
	{
		float* ViewMatrixPtr() { return glm::value_ptr(_viewMatrix); }
		float* ProjectionMatrixPtr() { return glm::value_ptr(_projectionMatrix); }

		explicit Camera(Window& window)
			: _window(window)
		{
			Reset();
		}

		void UpdateViewMatrix()
		{
			glm::vec3 focusPoint = _eyePosition + _directionVector;
			_viewMatrix = glm::lookAt(_eyePosition, focusPoint, glm::vec3(0, 1, 0)/*_upVector*/);
		}

		void UpdateProjectionMatrix()
		{
			_projectionMatrix = glm::perspective(45.0f, _window.Ratio(), 0.1f, 100.0f);
		}

		void UpdateDirection()
		{
			if (_pitch > 89.0f) _pitch = 89.0f;
			if (_pitch < -89.0f) _pitch = -89.0f;

			_directionVector.x = cos(glm::radians(_pitch)) * cos(glm::radians(_yaw));
			_directionVector.y = sin(glm::radians(_pitch));
			_directionVector.z = cos(glm::radians(_pitch)) * sin(glm::radians(_yaw));
			_directionVector = glm::normalize(_directionVector);

			_rightVector = glm::normalize(glm::cross(_upVector, _directionVector));
		}

		void Update() override
		{
			if (_window.GetKey(GLFW_KEY_LEFT)) _yaw--;
			if (_window.GetKey(GLFW_KEY_RIGHT)) _yaw++;
			if (_window.GetKey(GLFW_KEY_DOWN)) _pitch--;
			if (_window.GetKey(GLFW_KEY_UP)) _pitch++;

			glm::vec3 movement;
			if (_window.GetKey(GLFW_KEY_A)) { movement.x -= MOVEMENT_STEP; }
			if (_window.GetKey(GLFW_KEY_D)) { movement.x += MOVEMENT_STEP; }
			if (_window.GetKey(GLFW_KEY_W)) { movement.z -= MOVEMENT_STEP; }
			if (_window.GetKey(GLFW_KEY_S)) { movement.z += MOVEMENT_STEP; }
			if (_window.GetKey(GLFW_KEY_C)) { movement.y -= MOVEMENT_STEP; }
			if (_window.GetKey(GLFW_KEY_SPACE)) { movement.y += MOVEMENT_STEP; }

			Move(movement);

			UpdateDirection();
			UpdateViewMatrix();
			UpdateProjectionMatrix();
		}

		void Rotate(float dx, float dy)
		{
			_yaw += dx;
			_pitch += dy;

			UpdateDirection();
		}

		void Move(glm::vec3& movementVector)
		{
			_eyePosition += -movementVector.x * _rightVector;;
			_eyePosition += -movementVector.z * _directionVector;;
			_eyePosition += movementVector.y * _upVector;;
		}

		void OnMouseMove(double x, double y)
		{
			if (_firstMouseEvent)
			{
				_lastX = x;
				_lastY = y;
				_firstMouseEvent = false;
				return;
			}

			float dx = x - _lastX;
			float dy = _lastY - y;
			_lastX = x;
			_lastY = y;

			dx *= 0.05f;
			dy *= 0.05f;

			Rotate(dx, dy);
		}

		void OnKeyPress(int key, int action)
		{
		}

		void Reset()
		{
			_upVector = glm::vec3(0, 1, 0);
			_rightVector = glm::vec3(1, 0, 0);
			_directionVector = glm::vec3(0, 0, -1);
			_eyePosition = glm::vec3(0, 0, 2);
			_yaw = -90;
			_pitch = 0;

			UpdateDirection();
			UpdateViewMatrix();
			UpdateProjectionMatrix();
		}

		void Initialize() override
		{
		}

		virtual ~Camera() {}

		protected:

			openGL::Window& _window;

			glm::mat4 _viewMatrix;
			glm::mat4 _projectionMatrix;

			glm::vec3 _upVector;
			glm::vec3 _rightVector;
			glm::vec3 _directionVector;
			glm::vec3 _eyePosition;

			float _lastX, _lastY;
			float _pitch, _yaw;
			bool _firstMouseEvent = true;
	};
}

#endif // !OPENGL_CAMERA_INCLUDED
