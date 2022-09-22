#pragma once

#include <string>
#include <mainframe/math/vector2.h>
#include <mainframe/math/vector3.h>
#include <mainframe/math/matrix.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mainframe::game {
	class Window;

	class Camera3D {
		math::Vector3 location;
		math::Vector3 lookPos;
		math::Vector3 angle;
		math::Vector2i windowSize;

		float nearClippingPlane;
		float farClippingPlane;

		void updateLookat();

		glm::mat4x4 mat;
		glm::mat4x4 projmat;

	public:
		math::Vector3 getForward() const;
		math::Vector3 getLocation() const;
		const math::Vector3& getLookPos() const;
		math::Vector3 getAngle() const;
		const glm::mat4x4& getViewMatrix() const;
		const glm::mat4x4& getProjMatrix() const;

		void moveLocation(const math::Vector3& offset);

		void setLocation(const math::Vector3& pos);
		void lookAt(const math::Vector3& pos);
		void setAngle(const math::Vector3& angles);

		void setYaw(float yaw);
		float getYaw();

		void setViewMatrix(const glm::mat4x4& matrix);
		void setProjMatrix(const glm::mat4x4& matrix);

		void setWindowSize(const math::Vector2i& winsize);
		const math::Vector2i& getWindowSize();

		math::Vector3 worldToScreen(const math::Vector3& pos) const;
		math::Vector3 screenToWorld(const math::Vector2i& screen_pos) const;

		void insertViewMatrix(int location) const;

		Camera3D();
	};
}
