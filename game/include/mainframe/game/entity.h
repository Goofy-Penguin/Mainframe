#pragma once

#include <mainframe/math/matrix.h>
#include <vector>
#include <memory>

namespace mainframe {
	namespace game {
		class World;

		using EntityIdType = uint32_t;

		class Entity {
			World* world;
			mainframe::math::Matrix matrix;
			EntityIdType id = 0xFFFFFFFF;

		public:
			Entity(World* world_);
			virtual ~Entity();

			void remove();

			void setId(EntityIdType id_);
			EntityIdType getId() const;

			mainframe::math::Matrix& getMatrix();
			mainframe::math::Vector3 getPosition() const;
			mainframe::math::Vector3 getScale() const;
			mainframe::math::Vector3 getRotation() const;

			virtual void initialize();
			virtual void update();

			void generateUniqueId();

			std::vector<EntityIdType> getParentTreeList() const;

			template<class T>
			T* getWorld() const {
				return dynamic_cast<T*>(world);
			}
		};
	}
}
