#include "ISPHSolver.h"

using namespace Crystal::Physics;

std::vector<SPHParticle*> ISPHSolver::getParticles() const
{
	std::vector<SPHParticle*> results;
	for (const auto& object : objects) {
		const auto& particles = object->getParticles();
		results.insert(results.end(), particles.begin(), particles.end());
	}
	return results;
}