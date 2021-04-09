#pragma once

#include <string>
#include <mainframe/math/vector2.h>
#include <mainframe/math/vector3.h>
#include <mainframe/math/matrix.h>

namespace mainframe::game {
	class Window;

	class Camera3D {
		math::Vector3 location;
		math::Vector3 lookPos;
		math::Vector2 angle;

		float nearClippingPlane;
		float farClippingPlane;

		void updateLookat();

		math::Matrix mat;
		math::Matrix projmat;

	public:
		math::Vector3 getForward() const;
		const math::Vector3& getLocation() const;
		const math::Vector3& getLookPos() const;
		const math::Vector2& getAngle() const;
		const math::Matrix& getViewMatrix() const;
		const math::Matrix& getProjMatrix() const;

		void moveLocation(const math::Vector3& offset);

		void setLocation(const math::Vector3& pos);
		void lookAt(const math::Vector3& pos);
		void setAngle(const math::Vector2& angles);
		void setViewMatrix(const math::Matrix& matrix);
		void setProjMatrix(const math::Matrix& matrix);

		math::Vector3 worldToScreen(const math::Vector3& pos, const math::Vector2i& winsize) const;
		math::Vector3 screenToWorld(const math::Vector2i& screen_pos, const math::Vector2i& winsize) const;

		void insertViewMatrix(int location) const;

		Camera3D();
	};
}