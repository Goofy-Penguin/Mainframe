#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <mainframe/math/vector2.h>
#include <mainframe/math/vector3.h>

namespace mainframe::math {
	class Line {
	public:
		Vector2 a;
		Vector2 b;

		Line() = default;
		Line(const Vector2& a_, const Vector2& b_);

		bool intersects(const Line& other) const;
		bool intersects(const Line& other, Vector2& point) const;
		float length() const;
		Vector2 lerp(float timestep);

		bool operator== (const Line& other) const;
		bool operator!= (const Line& other) const;

		Line operator- (const Vector2& other) const;
		Line operator+ (const Vector2& other) const;
		Line operator/ (const Vector2& other) const;
		Line operator* (const Vector2& other) const;
		Line& operator-= (const Vector2& other);
		Line& operator+= (const Vector2& other);
		Line& operator*= (const Vector2& other);
		Line& operator/= (const Vector2& other);

		Line operator- (float other) const;
		Line operator+ (float other) const;
		Line operator/ (float other) const;
		Line operator* (float other) const;
		Line& operator-= (float other);
		Line& operator+= (float other);
		Line& operator*= (float other);
		Line& operator/= (float other);
	};
}