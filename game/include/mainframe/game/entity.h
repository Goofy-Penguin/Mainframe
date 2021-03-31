#pragma once

#include <mainframe/math/matrix.h>
#include <vector>
#include <memory>

namespace mainframe {
	namespace game {
		class World;

		class Entity {
			World* world;
			mainframe::math::Matrix matrix;
			size_t id;

		public:
			Entity(World* world_);
			virtual ~Entity();

			void remove();

			void setId(size_t id_);
			size_t getId() const;
			mainframe::math::Matrix& getMatrix();

			void generateUniqueId();

			template<class T>
			T* getWorld() {
				return dynamic_cast<T*>(world);
			}
		};
	}
}
