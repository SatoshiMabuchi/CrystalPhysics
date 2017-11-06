#ifndef __CRYSTAL_PHYSICS_PHYSICS_WORLD_H__
#define __CRYSTAL_PHYSICS_PHYSICS_WORLD_H__

#include "PhysicsObject.h"
#include "SPHKernel.h"
#include "../../Crystal/Math/Box3d.h"
#include <vector>

namespace Crystal {
	namespace Physics {
		class PhysicsObject;

		class PhysicsWorld
		{
		public:
			PhysicsWorld() : timeStep(0)
			{}

			void clear();

			void add(PhysicsObject* object) { objects.push_back(object); }

			void simulate(const float effectLength, const float timeStep);

			void setBoundary(const Math::Box3d& boundary) { this->boundary = boundary; }

			void setExternalForce(const Math::Vector3df& force) { this->externalForce = force; }

			std::vector< SPHParticle*> getParticles() const;

			int getTimeStep() const { return timeStep; }

		private:
			int timeStep;
			std::vector< PhysicsObject* > objects;
			Math::Vector3df externalForce;
			SPHKernel kernel;
			Math::Box3d boundary;
		};

	}
}

#endif