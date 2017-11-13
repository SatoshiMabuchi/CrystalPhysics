#ifndef __CRYSTAL_PHYSICS_PHYSICS_WORLD_H__
#define __CRYSTAL_PHYSICS_PHYSICS_WORLD_H__

#include "SPHKernel.h"
#include "../../Crystal/Math/Box3d.h"
#include "ISPHSolver.h"
#include <vector>

namespace Crystal {
	namespace Physics {
		class PhysicsObject;

class STSPHSolver : public ISPHSolver
{
public:
	STSPHSolver() : timeStep(0)
	{}

	void clear();

	void simulate(const float effectLength, const float timeStep);

	void setBoundary(const Math::Box3d& boundary) { this->boundary = boundary; }

	void setExternalForce(const Math::Vector3df& force) { this->externalForce = force; }

	int getTimeStep() const { return timeStep; }

private:
	int timeStep;
	Math::Vector3df externalForce;
	SPHKernel kernel;
	Math::Box3d boundary;
};

	}
}

#endif