#ifndef __CRYSTAL_PHYSICS_PHYSICS_WORLD_H__
#define __CRYSTAL_PHYSICS_PHYSICS_WORLD_H__

#include "SPHKernel.h"
#include "../../Crystal/Math/Box3d.h"
#include "ISPHSolver.h"
#include "SPHParticle.h"
#include <vector>

namespace Crystal {
	namespace Physics {
		class PhysicsObject;

class STSPHSolver : public ISPHSolver
{
public:
	STSPHSolver() : timeStep(0)
	{}

	void add(SPHParticle* particle) {
		this->particles.push_back(particle);
	}

	void clear();

	void simulate(const float effectLength, const float timeStep);

	int getTimeStep() const { return timeStep; }

	std::vector<SPHParticle*> getParticles() const { return particles; }

private:
	int timeStep;
	SPHKernel kernel;
	std::vector<SPHParticle*> particles;
};

	}
}

#endif