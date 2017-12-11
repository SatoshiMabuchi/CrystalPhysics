#include "DFSPHSolver.h"

#include "IndexedFinder.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void DFSPHSolver::simulate(const float dt, const float effectLength)
{
	const auto& particles = getParticles();

	for (const auto& particle : particles) {
		particle->init();
	}

	IndexedFinder algo(effectLength);
	algo.add(particles);
	algo.createPairs(particles);
	auto& pairs = algo.getPairs();

	/*

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		pairs[i].solveDensity();
	}
	*/

	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		particles[i]->addSelfDensity();
	}

	// compute factors.

	// while( t < t_max ) {
	//	compute non-pressure forces.
	//}

}
