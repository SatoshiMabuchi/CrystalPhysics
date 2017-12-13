#ifndef __CRYSTAL_PHYSICS_PBSPH_SOLVER_H__
#define __CRYSTAL_PHYSICS_PBSPH_SOLVER_H__

#include <vector>

#include "../../Crystal/Math/Vector3d.h"
#include "../../Crystal/Math/Box3d.h"

namespace Crystal {
	namespace Physics {
		class PBSPHParticle;
		class PBSPHObject;

class PBSPHSolver
{
public:
	PBSPHSolver()
	{}

	void add(PBSPHParticle* particle) { this->particles.push_back(particle); }

	void simulate(const float dt, const float effectRadius, const float searchRadius, const int maxIter);

	void setBoundary(const Math::Box3d& boundary) { this->boundary = boundary; }

	void setExternalForce(const Math::Vector3df& force) { this->externalForce = force; }

	std::vector<PBSPHParticle*> getParticles() const { return particles; }

private:
	std::vector<PBSPHParticle*> particles;
	Math::Box3d boundary;
	Math::Vector3df externalForce;
};

	}
}

#endif