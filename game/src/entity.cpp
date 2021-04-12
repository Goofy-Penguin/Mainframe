#include <mainframe/game/entity.h>
#include <mainframe/game/world.h>

namespace mainframe {
	namespace game {
		Entity::Entity(World* world_) : world(world_) {

		}

		std::vector<EntityIdType> Entity::getParentTreeList() const {
			std::vector<EntityIdType> ret;

			// if the `world` is convertable to an entity, it means that the world has a parent world.
			// if you store a box inside a closet in a house, we have world->house->closet->box
			World* current = world;

			do {
				auto container = dynamic_cast<Entity*>(current);
				if (container == nullptr) break;
				ret.insert(ret.begin(), container->getId());

				current = container->getWorld<World>();
			} while (current != nullptr);

			ret.push_back(id);
			return ret;
		}


		Entity::~Entity() {

		}

		void Entity::remove() {
			world->removeEntity(this);
		}

		EntityIdType Entity::getId() const {
			return id;
		}

		void Entity::setId(EntityIdType id_) {
			id = id_;
		}

		void Entity::generateUniqueId() {
			static EntityIdType _id = 0;

			id = ++_id;
		}

		void Entity::initialize() {

		}

		void Entity::update() {

		}

		mainframe::math::Matrix& Entity::getMatrix() {
			return matrix;
		}

		mainframe::math::Vector3 Entity::getPosition() const {
			return matrix.getTranslation();
		}

		mainframe::math::Vector3 Entity::getScale() const {
			return matrix.getScale();
		}

		mainframe::math::Vector3 Entity::getRotation() const {
			return matrix.getRotation();
		}
	}
}
