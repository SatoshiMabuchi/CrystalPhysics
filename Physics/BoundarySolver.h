#ifndef __CRYSTAL_PHYSICS_BOUNDARY_SOLVER_H__
#define __CRYSTAL_PHYSICS_BOUNDARY_SOLVER_H__

#include "../../Crystal/Math/Box3d.h"

namespace Crystal {
	namespace Physics {
		class SPHParticle;

class BoundarySolver
{
public:
	BoundarySolver() :
		timeStep(1.0f) {};

	BoundarySolver(const float timeStep, const Math::Box3d& boundary);

	void solve(const std::vector<SPHParticle*>& particles);

private:
	Math::Vector3df getBoundaryForce(const Math::Vector3df& center);

	Math::Vector3df getForceX(const float x);
	Math::Vector3df getForceY(const float y);
	Math::Vector3df getForceZ(const float z);

	float getForce(const float over);

	const float timeStep;
	const Math::Box3d boundary;
};
	}
}

#endif