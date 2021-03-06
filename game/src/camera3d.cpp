#include <mainframe/game/camera3d.h>
#include <mainframe/numbers/pi.h>

namespace mainframe::game {
	void Camera3D::updateLookat() {
		lookPos = {
			location.x + std::sin(angle.y) * std::cos(angle.x),
			location.y + std::sin(angle.y) * std::sin(angle.x),
			location.z + std::cos(angle.y)
		};

		this->mat = math::Matrix::createLookAt(location, lookPos, {0.0, 1.0, 0.0});
	}

	Camera3D::Camera3D() : location(0, 4, 0), angle(numbers::pi<float> / 2, numbers::pi<float> / 2) {
		updateLookat();
	}

	math::Vector3 Camera3D::getForward() const {
		return {
			std::sin(angle.x) * std::cos(angle.y),
			std::cos(angle.x),
			std::sin(angle.x) * std::sin(angle.y)
		};
	}

	math::Vector3 Camera3D::getLocation() const {
		return location.xzy();
	}

	const math::Vector3& Camera3D::getLookPos() const {
		return lookPos;
	}

	math::Vector2 Camera3D::getAngle() const {
		return angle;
	}

	const math::Matrix& Camera3D::getViewMatrix() const {
		return mat;
	}

	const math::Matrix& Camera3D::getProjMatrix() const {
		return projmat;
	}

	void Camera3D::moveLocation(const math::Vector3& offset) {
		location += offset.xzy();
		updateLookat();
	}

	void Camera3D::setLocation(const math::Vector3& pos) {
		location = pos.xzy();
		updateLookat();
	}

	void Camera3D::lookAt(const math::Vector3& pos) {
		this->mat = math::Matrix::createLookAt(location, pos, {0.0, 1.0, 0.0});
	}

	void Camera3D::setAngle(const math::Vector2& angles) {
		angle = angles;
		updateLookat();
	}

	void Camera3D::setViewMatrix(const math::Matrix& matrix) {
		mat = matrix;
	}

	void Camera3D::setProjMatrix(const math::Matrix& matrix) {
		projmat = matrix;
	}

	void Camera3D::setWindowSize(const math::Vector2i& winsize) {
		windowSize = winsize;
	}

	int glhProjectf(float objx, float objy, float objz, std::array<float, 16> modelview, std::array<float, 16> projection, int* viewport, float* windowCoordinate) {
		//Transformation vectors
		float fTempo[8];
		//Modelview transform
		fTempo[0] = modelview[0] * objx + modelview[4] * objy + modelview[8] * objz + modelview[12];  //w is always 1
		fTempo[1] = modelview[1] * objx + modelview[5] * objy + modelview[9] * objz + modelview[13];
		fTempo[2] = modelview[2] * objx + modelview[6] * objy + modelview[10] * objz + modelview[14];
		fTempo[3] = modelview[3] * objx + modelview[7] * objy + modelview[11] * objz + modelview[15];
		//Projection transform, the final row of projection matrix is always [0 0 -1 0]
		//so we optimize for that.
		fTempo[4] = projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8] * fTempo[2] + projection[12] * fTempo[3];
		fTempo[5] = projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9] * fTempo[2] + projection[13] * fTempo[3];
		fTempo[6] = projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
		fTempo[7] = -fTempo[2];
		//The result normalizes between -1 and 1
		if (fTempo[7] == 0.0f)	//The w value
			return 0;

		fTempo[7] = 1.0f / fTempo[7];
		//Perspective division
		fTempo[4] *= fTempo[7];
		fTempo[5] *= fTempo[7];
		fTempo[6] *= fTempo[7];
		//Window coordinates
		//Map x, y to range 0-1
		windowCoordinate[0] = (fTempo[4] * 0.5f + 0.5f) * viewport[2] + viewport[0];
		windowCoordinate[1] = (fTempo[5] * -0.5f + 0.5f) * viewport[3] + viewport[1];
		//This is only correct when glDepthRange(0.0, 1.0)
		windowCoordinate[2] = (1.0f + fTempo[6]) * 0.5f;	//Between 0 and 1
		return 1;
	}

	const math::Vector2i& Camera3D::getWindowSize() {
		return windowSize;
	}

	math::Vector3 Camera3D::worldToScreen(const math::Vector3& pos) const {
		int viewport[] = {0, 0, (int)windowSize.x, (int)windowSize.y};

		math::Vector3 sp;
		glhProjectf(pos.x, pos.z, pos.y, mat.getValues(), projmat.getValues(), viewport, (float*)&sp);

		return sp;
	}

	math::Vector3 Camera3D::screenToWorld(const math::Vector2i& screen_pos) const {
		math::Matrix viewproj_inv = (projmat * mat).inverted();

		float screenx_clip = 2 * (static_cast<float>(screen_pos.x) / static_cast<float>(windowSize.x)) - 1;
		float screeny_clip = -(1 - 2 * static_cast<float>(screen_pos.y) / static_cast<float>(windowSize.y));

		math::Vector4 screen_clip = {screenx_clip, screeny_clip, -1, 1};
		math::Vector4 world_pos = viewproj_inv.translate(screen_clip);

		world_pos /= world_pos.w;

		return {world_pos.x, world_pos.y, world_pos.z};
	}
}
