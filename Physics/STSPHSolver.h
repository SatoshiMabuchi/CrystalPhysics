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

	int getTimeStep() const { return timeStep; }

private:
	int timeStep;
	SPHKernel kernel;
};

	}
}

#endif