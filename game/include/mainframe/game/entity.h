#pragma once

#include <mainframe/math/matrix.h>
#include <vector>
#include <memory>

namespace mainframe::game {
	class World;

	using EntityIdType = uint32_t;

	class Entity {
		World* world = nullptr;
		mainframe::math::Matrix matrix;
		mainframe::math::Vector3 velocity;
		EntityIdType id = 0xFFFFFFFF;

	public:
		Entity();
		Entity(World* world_);
		virtual ~Entity();

		virtual void remove();

		void setId(EntityIdType id_);
		EntityIdType getId() const;

		math::Matrix& getMatrix();
		const math::Matrix& getMatrix() const;

		virtual math::Vector3 getPosition() const;
		virtual math::Vector3 getScale() const;
		virtual math::Vector3 getRotation() const;
		virtual math::Vector3 getVelocity() const;

		virtual void setVelocity(const math::Vector3& force);
		virtual void setPosition(const math::Vector3& pos);
		virtual void setScale(const math::Vector3& scale);
		virtual void setRotation(const math::Vector3& rot);

		virtual void initialize();
		virtual void update(float deltaTime);
		virtual void fixedUpdate(float deltaTime);

		void generateUniqueId();

		std::vector<EntityIdType> getParentTreeList() const;

		template<class T = World>
		T* getWorld() const {
			return dynamic_cast<T*>(world);
		}

		virtual void setWorld(World* world_);
	};
}
