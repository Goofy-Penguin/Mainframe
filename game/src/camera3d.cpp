#include <mainframe/game/camera3d.h>
#include <mainframe/numbers/pi.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace mainframe::game {
	Camera3D::Camera3D() : location(0, 0, 0), angle(0, 0, 0) {
		updateLookat();
	}

	void Camera3D::updateLookat() {
		lookPos = getForward();

		auto worldUp = glm::vec3(0, 0, 1);
		auto front = glm::normalize(glm::vec3(lookPos.x, lookPos.y, lookPos.z));
		auto right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        auto up = glm::normalize(glm::cross(right, front));

		auto glmLocation = glm::vec3(location.x, location.y, location.z);

		viewMat = glm::lookAt(
			glmLocation,
			glmLocation + front,
			up
		);
	}

	void Camera3D::setYaw(float yaw) {
		angle.y = yaw;
		updateLookat();
	}

	float Camera3D::getYaw() {
		return angle.y;
	}

	math::Vector3 Camera3D::getForward() const {
		return {
			std::cos(angle.x) * std::cos(angle.y),
			std::sin(angle.y),
			std::sin(angle.x) * std::cos(angle.y)
		};
	}

	math::Vector3 Camera3D::getLocation() const {
		return location;
	}

	const math::Vector3& Camera3D::getLookPos() const {
		return lookPos;
	}

	math::Vector3 Camera3D::getAngle() const {
		return angle;
	}

	const glm::mat4x4& Camera3D::getViewMatrix() const {
		return viewMat;
	}

	const glm::mat4x4& Camera3D::getProjMatrix() const {
		return projMat;
	}

	void Camera3D::moveLocation(const math::Vector3& offset) {
		location += offset;
		updateLookat();
	}

	void Camera3D::setLocation(const math::Vector3& pos) {
		location = pos;
		updateLookat();
	}

	/*
		x = yaw
		y = pitch
	*/
	void Camera3D::lookAt(const math::Vector3& pos) {
		viewMat = glm::lookAt(
			glm::vec3(location.x, location.y, location.z),
			glm::vec3(pos.x, pos.y, pos.z),
			glm::vec3(0, 0, 1)
		);
	}

	void Camera3D::setAngle(const math::Vector3& angles) {
		angle = angles;
		updateLookat();
	}

	void Camera3D::setViewMatrix(const glm::mat4x4& matrix) {
		viewMat = matrix;
	}

	void Camera3D::setProjMatrix(const glm::mat4x4& matrix) {
		projMat = matrix;
	}

	void Camera3D::setWindowSize(const math::Vector2i& winsize) {
		windowSize = winsize;
	}

	const math::Vector2i& Camera3D::getWindowSize() {
		return windowSize;
	}

	math::Vector3 Camera3D::worldToScreen(const math::Vector3& pos) const {
		glm::vec4 viewport = {0, (int)windowSize.y, (int)windowSize.x, -(int)windowSize.y};
		auto out = glm::project(glm::vec3(pos.x, pos.y, pos.z), viewMat, projMat, viewport);

		return {out.x, out.y, out.z};
	}

	math::Vector3 Camera3D::screenToWorld(const math::Vector2i& screen_pos) const {
		glm::vec4 viewport = {0, 0, (int)windowSize.x, (int)windowSize.y};
		//glm::inverse(viewMat * projMat)
		auto out = glm::unProject(glm::vec3(screen_pos.x, screen_pos.y, 1.f), viewMat * glm::mat4(1.f), projMat, viewport);
		return {out.x, out.y, 1.f};
	}
}
