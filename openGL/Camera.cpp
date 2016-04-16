#define MOVEMENT_STEP 0.5f

#include "Camera.h"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace openGL
{
	/// Getters
	float* Camera::ViewMatrixPtr() { return glm::value_ptr(_viewMatrix); }
	float* Camera::ProjectionMatrixPtr() { return glm::value_ptr(_projectionMatrix); }
	const glm::mat4& Camera::ViewMatrix() const { return _viewMatrix; }
	const glm::mat4& Camera::ProjectionMatrix() const { return _projectionMatrix; }
	glm::mat4 Camera::ViewProjectionMatrix() const { return _projectionMatrix * _viewMatrix; }
	/// -------

	Camera::Camera(Window& window)
		: _window(window)
	{
		Reset();
	}

	void Camera::Initialize()
	{
	}

	void Camera::Update()
	{
		if (GetKey(GLFW_KEY_LEFT)) _yaw--;
		if (GetKey(GLFW_KEY_RIGHT)) _yaw++;
		if (GetKey(GLFW_KEY_DOWN)) _pitch--;
		if (GetKey(GLFW_KEY_UP)) _pitch++;

		const int speed = _window.GetKey(GLFW_KEY_LEFT_SHIFT) ? 2 : 1;

		glm::vec3 movement;
		if (GetKey(GLFW_KEY_A)) { movement.x += MOVEMENT_STEP * speed; }
		if (GetKey(GLFW_KEY_D)) { movement.x -= MOVEMENT_STEP * speed; }
		if (GetKey(GLFW_KEY_W)) { movement.z += MOVEMENT_STEP * speed; }
		if (GetKey(GLFW_KEY_S)) { movement.z -= MOVEMENT_STEP * speed; }
		if (GetKey(GLFW_KEY_C)) { movement.y -= MOVEMENT_STEP * speed; }
		if (GetKey(GLFW_KEY_SPACE)) { movement.y += MOVEMENT_STEP * speed; }

		Move(movement);

		UpdateDirection();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::vec3 focusPoint = _position + _direction;
		_viewMatrix = glm::lookAt(_position, focusPoint, glm::vec3(0, 1, 0)/*_upVector*/);
	}

	void Camera::UpdateProjectionMatrix()
	{
		_projectionMatrix = glm::perspective(45.0f, _window.Ratio(), 0.1f, 10000.0f);
	}

	void Camera::UpdateDirection()
	{
		if (_pitch > 89.0f) _pitch = 89.0f;
		if (_pitch < -89.0f) _pitch = -89.0f;

		this->Entity::UpdateDirection();
	}

	void Camera::Rotate(float dx, float dy)
	{
		_yaw += dx;
		_pitch += dy;

		UpdateDirection();
	}

	void Camera::Reset()
	{
		_up = glm::vec3(0, 1, 0);
		_right = glm::vec3(1, 0, 0);
		_direction = glm::vec3(0, 0, -1);
		_position = glm::vec3(0, 0, 0);
		_yaw = -90;
		_pitch = 0;

		UpdateDirection();
		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::OnMouseMove(double x, double y)
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


	void Camera::OnKeyPress(int key, int action)
	{
	}

	int Camera::GetKey(int key) const
	{
		return _window.GetKey(key);
	}


	Camera::~Camera()
	{
	}
}