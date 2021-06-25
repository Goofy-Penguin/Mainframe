#include <mainframe/game/entity.h>
#include <mainframe/game/world.h>

using namespace mainframe::math;

namespace mainframe {
	namespace game {
		Entity::Entity() { }
		Entity::Entity(World* world_) : world(world_) { }

		void Entity::setWorld(World* world_) {
			world = world_;
		}

		std::vector<EntityIdType> Entity::getParentTreeList() const {
			std::vector<EntityIdType> ret;
			if (world == nullptr) return ret;

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
			if (world == nullptr) return;

			world->removeEntity(this);
			world = nullptr;
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

		void Entity::update(float deltaTime) { }

		Matrix& Entity::getMatrix() {
			return matrix;
		}

		const Matrix& Entity::getMatrix() const {
			return matrix;
		}

		Vector3 Entity::getPosition() const {
			return matrix.getTranslation();
		}

		Vector3 Entity::getScale() const {
			return matrix.getScale();
		}

		Vector3 Entity::getRotation() const {
			return matrix.getRotation();
		}

		Vector3 Entity::getVelocity() const {
			return velocity;
		}

		void Entity::setVelocity(const Vector3& force) {
			velocity = force;
		}

		void Entity::setPosition(const Vector3& pos) {
			auto rot = getRotation();

			matrix = Matrix::createTranslation(pos) *
				Matrix::createRotationZ(rot.z) *
				Matrix::createRotationX(rot.x) *
				Matrix::createRotationY(rot.y) *
				Matrix::createScale(getScale());
		}

		void Entity::setScale(const Vector3& scale) {
			auto rot = getRotation();

			matrix = Matrix::createTranslation(getPosition()) *
				Matrix::createRotationZ(rot.z) *
				Matrix::createRotationX(rot.x) *
				Matrix::createRotationY(rot.y) *
				Matrix::createScale(scale);
		}

		void Entity::setRotation(const Vector3& rot){
			matrix = Matrix::createTranslation(getPosition()) *
				Matrix::createRotationZ(rot.z) *
				Matrix::createRotationX(rot.x) *
				Matrix::createRotationY(rot.y) *
				Matrix::createScale(getScale());
		}
	}
}
