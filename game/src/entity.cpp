#include <mainframe/game/entity.h>
#include <mainframe/game/world.h>

namespace mainframe {
	namespace game {
		Entity::Entity(World* world_) : world(world_) {

		}

		Entity::~Entity() {

		}

		void Entity::remove() {
			world->removeEntity(this);
		}

		size_t Entity::getId() const {
			return id;
		}

		void Entity::setId(size_t id_) {
			id = id_;
		}

		void Entity::generateUniqueId() {
			static size_t _id = 0;

			id = ++_id;
		}

		mainframe::math::Matrix& Entity::getMatrix() {
			return matrix;
		}
	}
}
