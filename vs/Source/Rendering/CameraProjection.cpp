#include "stdafx.h"
#include "CameraProjection.h"

#include "Camera.h"

// Static

std::vector<std::shared_ptr<Tet::CameraProjection>> Tet::CameraProjection::_cameraProjection{
    std::make_shared<Tet::PerspectiveProjection>(),
    std::make_shared<Tet::OrthographicProjection>()
};

float Tet::CameraProjection::CameraProperties::computeAspect() const
{
	return static_cast<float>(_width) / static_cast<float>(_height);
}

void Tet::CameraProjection::CameraProperties::computeAxes(vec3& n, vec3& u, vec3& v) const
{
	n = glm::normalize(_eye - _lookAt);							// z axis

	if (glm::all(glm::epsilonEqual(n, -_up, glm::epsilon<float>())))		// x axis: UP x n is 0 as both vectors are parallel. Since up and n are normalized we can check if they are equal (with epsilon checkup)
	{
		u = glm::normalize(glm::cross(vec3(0.0f, 0.0f, -1.0f), n));
	}
	else if (glm::all(glm::epsilonEqual(n, _up, glm::epsilon<float>())))
	{
		u = glm::normalize(glm::cross(vec3(0.0f, 0.0f, 1.0f), n));
	}
	else
	{
		u = glm::normalize(glm::cross(_up, n));
	}
	v = glm::normalize(glm::cross(n, u));					// y axis
}

vec2 Tet::CameraProjection::CameraProperties::computeBottomLeftCorner()
{
	const float halfWidth = _width / 2.0f;
	const float halfHeight = _height / 2.0f;

	return vec2(-halfWidth, -halfHeight);
}

float Tet::CameraProjection::CameraProperties::computeFovY()
{
	return 2.0f * glm::atan(glm::tan(_fovX / 2.0f) / _aspect);
}

void Tet::CameraProjection::CameraProperties::computeProjectionMatrices(CameraProperties* camera)
{
	_projectionMatrix = _cameraProjection[_cameraType]->buildProjectionMatrix(camera);
	_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void Tet::CameraProjection::CameraProperties::computeViewMatrices()
{
	this->computeViewMatrix();
	_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void Tet::CameraProjection::CameraProperties::computeViewMatrix()
{
	_viewMatrix = glm::lookAt(_eye, _lookAt, _up);
}

void Tet::CameraProjection::CameraProperties::zoom(float speed)
{
	CameraProjection::_cameraProjection[this->_cameraType]->zoom(this, speed);
}

// Projection

mat4 Tet::PerspectiveProjection::buildProjectionMatrix(CameraProperties* camera)
{
    return glm::perspective(camera->_fovY, camera->_aspect, camera->_zNear, camera->_zFar);
}

void Tet::PerspectiveProjection::zoom(CameraProperties* camera, const float speed)
{
	float angle = camera->_fovY - speed;
	if (angle < glm::pi<float>() && angle > 0.0f)
	{
		camera->_fovY = angle;
		camera->computeProjectionMatrices(camera);
	}
}

mat4 Tet::OrthographicProjection::buildProjectionMatrix(CameraProperties* camera)
{
	glm::vec2 bottomLeftCorner = camera->_bottomLeftCorner;
	return glm::ortho(bottomLeftCorner.x, -bottomLeftCorner.x, bottomLeftCorner.y, -bottomLeftCorner.y, camera->_zNear, camera->_zFar);
}

void Tet::OrthographicProjection::zoom(CameraProperties* camera, const float speed)
{
	const float units = -speed;
	const float raspect = camera->_aspect;
	glm::vec2 bottomLeftCorner = camera->_bottomLeftCorner;

	if (bottomLeftCorner.x - units >= 0 || bottomLeftCorner.y - units >= 0) return;

	bottomLeftCorner -= vec2(units * raspect, units);
	camera->_bottomLeftCorner = bottomLeftCorner;

	camera->computeProjectionMatrices(camera);
}
