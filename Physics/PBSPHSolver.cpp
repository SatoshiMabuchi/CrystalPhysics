#include "PBSPHSolver.h"
#include "PBSPHParticle.h"

#include "IndexedFinder.h"
#include "PBSPHBoundarySolver.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void PBSPHSolver::simulate(const float dt, const float effectLength, const float searchLength, const int maxIter)
{
	/*
	float maxVelocity = 0.0;
	for (auto p : particles) {
		maxVelocity = std::max<float>(maxVelocity, glm::length(p->getVelocity()));
	}
	auto dt = 0.2 * (1.0f / maxVelocity);
	dt = std::max<float>(dt, 0.02);
	dt = std::min<float>(dt, 0.2);
	*/

	for (auto p : particles) {
		p->init();
	}

	PBSPHBoundarySolver boundarySolver(boundary);
	boundarySolver.solveForce(particles, dt);
	/*
	std::list<PBSPHParticle*> neighbors;
	for (auto p : particles) {
		if (boundarySolver.isBoundary(p)) {
			auto neighbor = boundarySolver.generateBoundaryParticle(p);
			neighbors.push_back(neighbor);
			p->addNeighbor(neighbor);
		}
	}
	*/

	for (auto p : particles) {
		p->addExternalForce(externalForce);
		p->predictPosition(dt);
	}

	IndexedFinder finder(searchLength);
	for (auto p : particles) {
		finder.add(p);
	}
	finder.createPairs();
	const auto& pairs = finder.getPairs();
	for (auto p : pairs) {
		auto p1 = static_cast<PBSPHParticle*>(p.getParticle1());
		auto p2 = static_cast<PBSPHParticle*>(p.getParticle2());
		p1->addNeighbor(p2);
		p2->addNeighbor(p1);
	}

	SPHKernel kernel(effectLength);
	for (auto p : particles) {
		p->setKernel(&kernel);
	}

	for (int iter = 0; iter < maxIter; ++iter) {
#pragma omp parallel for
		for (int i = 0; i < particles.size(); ++i) {
			const auto p = particles[i];
			p->solveDensity();
		}
		boundarySolver.solveDensity(particles);
#pragma omp parallel for
		for (int i = 0; i < particles.size(); ++i) {
			const auto p = particles[i];
			p->solveConstrantGradient();
		}
		boundarySolver.solveConstraintGradient(particles);
#pragma omp parallel for
		for (int i = 0; i < particles.size(); ++i) {
			const auto p = particles[i];
			p->solveDensityConstraint();
		}

#pragma omp parallel for
		for (int i = 0; i < particles.size(); ++i) {
			const auto p = particles[i];
			p->solvePositionCorrection();
		}
		boundarySolver.solveCorrectPosition(particles);
		for (int i = 0; i < particles.size(); ++i) {
			const auto p = particles[i];
			p->updatePredictPosition(dt);
		}
	}

	/*
	for (auto p : pairs) {
		p.solveNormal();
	}
	*/

	//boundarySolver.solveViscosity(particles);

	for (auto p : particles) {
		p->updateVelocity(dt);
		p->solveViscosity();
		p->updateViscosity();
		p->updatePosition();
		//	p->integrate(dt);
	}

	/*
	for (auto n : neighbors) {
		delete n;
	}
	*/
}