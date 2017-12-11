#include "STSPHSolver.h"
#include "SPHParticle.h"
#include "BoundarySolver.h"

#include "IndexedFinder.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void STSPHSolver::clear()
{
	timeStep = 0;
	for (auto p : particles) {
		delete p;
	}
	particles.clear();
}

void STSPHSolver::simulate(const float effectLength, const float timeStep)
{
	this->timeStep++;

	for (auto particle : particles) {
		particle->init();
	}

	IndexedFinder algo(effectLength);
	for (auto p : particles) {
		algo.add(p);
	}
	algo.createPairs();
	auto& pairs = algo.getPairs();

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		auto p1 = static_cast<SPHParticle*>(pairs[i].getParticle1());
		auto p2 = static_cast<SPHParticle*>(pairs[i].getParticle2());
		p1->addDensity(*p2);
		p2->addDensity(*p1);
	}

	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		particles[i]->addSelfDensity();
	}

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		auto p1 = static_cast<SPHParticle*>(pairs[i].getParticle1());
		auto p2 = static_cast<SPHParticle*>(pairs[i].getParticle2());
		p1->solvePressureForce(*p2);
		p2->solvePressureForce(*p1);
		p1->solveViscosityForce(*p2);
		p2->solveViscosityForce(*p1);
	}

	for (auto p : particles) {
		p->addExternalForce(externalForce);
	}

	BoundarySolver boundarySolver(timeStep, boundary);
	boundarySolver.solve(particles);


	for (auto p : particles) {
		p->forwardTime(timeStep);
	}
}
