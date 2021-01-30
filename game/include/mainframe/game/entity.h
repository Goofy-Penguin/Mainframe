#pragma once

#include <mainframe/math/matrix.h>
#include <vector>
#include <memory>

namespace mainframe {
	namespace game {
		class World;

		class Entity {
			World& world;
			mainframe::math::Matrix matrix;
			std::weak_ptr<Entity> reference;
			size_t id;

		public:
			Entity(World& world_);
			~Entity();

			void remove();

			size_t getId() const;
			mainframe::math::Matrix& getMatrix();
			const std::weak_ptr<Entity>& getReference() const;

			void setReference(const std::weak_ptr<Entity>& reference_);
		};
	}
}
