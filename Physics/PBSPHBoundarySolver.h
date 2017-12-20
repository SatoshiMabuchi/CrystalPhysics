#ifndef __CRYSTAL_PHYSICS_PBSPH_BOUNDARY_SOLVER_H__
#define __CRYSTAL_PHYSICS_PBSPH_BOUNDARY_SOLVER_H__

#include "../../Crystal/Math/Vector3d.h"
#include "../../Crystal/Math/Box3d.h"

namespace Crystal {
	namespace Physics {
		class PBSPHParticle;

class PBSPHBoundarySolver
{
public:
	PBSPHBoundarySolver() {};

	PBSPHBoundarySolver(const Math::Box3d& boundary);

	void solveForce(const std::vector<PBSPHParticle*>& particles, const float dt);

private:
	bool isBoundary(PBSPHParticle* particle);

	Math::Vector3df getBoundaryPosition(PBSPHParticle* particle);

	Math::Vector3df getOverVector(const Math::Vector3df& position);

	float getOverX(const float x);
	float getOverY(const float y);
	float getOverZ(const float z);

	const Math::Box3d boundary;
};
	}
}

#endif