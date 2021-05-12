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
			mainframe::math::Vector3 velocity;
			EntityIdType id = 0xFFFFFFFF;

		public:
			Entity(World* world_);
			virtual ~Entity();

			virtual void remove();

			void setId(EntityIdType id_);
			EntityIdType getId() const;

			mainframe::math::Matrix& getMatrix();
			const mainframe::math::Matrix& getMatrix() const;
			virtual mainframe::math::Vector3 getPosition() const;
			virtual mainframe::math::Vector3 getScale() const;
			virtual mainframe::math::Vector3 getRotation() const;
			virtual mainframe::math::Vector3 getVelocity() const;

			virtual void setVelocity(const mainframe::math::Vector3& force);
			virtual void setPosition(const mainframe::math::Vector3& pos);
			virtual void setScale(const mainframe::math::Vector3& scale);
			virtual void setRotation(const mainframe::math::Vector3& rot);

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
