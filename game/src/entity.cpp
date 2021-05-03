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

		mainframe::math::Vector3 Entity::getVelocity() const {
			return velocity;
		}

		void Entity::setVelocity(const mainframe::math::Vector3& force) {
			velocity = force;
		}

		void Entity::setPosition(const mainframe::math::Vector3& pos) {
			auto rot = getRotation();

			matrix = mainframe::math::Matrix::createTranslation(pos) *
				mainframe::math::Matrix::createRotationZ(rot.z) *
				mainframe::math::Matrix::createRotationX(rot.x) *
				mainframe::math::Matrix::createRotationY(rot.y) *
				mainframe::math::Matrix::createScale(getScale());
		}

		void Entity::setScale(const mainframe::math::Vector3& scale) {
			auto rot = getRotation();

			matrix = mainframe::math::Matrix::createTranslation(getPosition()) *
				mainframe::math::Matrix::createRotationZ(rot.z) *
				mainframe::math::Matrix::createRotationX(rot.x) *
				mainframe::math::Matrix::createRotationY(rot.y) *
				mainframe::math::Matrix::createScale(scale);
		}

		void Entity::setRotation(const mainframe::math::Vector3& rot){
			matrix = mainframe::math::Matrix::createTranslation(getPosition()) *
				mainframe::math::Matrix::createRotationZ(rot.z) *
				mainframe::math::Matrix::createRotationX(rot.x) *
				mainframe::math::Matrix::createRotationY(rot.y) *
				mainframe::math::Matrix::createScale(getScale());
		}
	}
}
