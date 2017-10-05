#include "PhysicsWorld.h"
#include "SPHParticle.h"
#include "BoundarySolver.h"

#include "IndexedFinder.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void PhysicsWorld::clear()
{
	timeStep = 0;
	objects.clear();
}

void PhysicsWorld::simulate(const float effectLength, const float timeStep)
{
	this->timeStep++;

	const auto& particles = getParticles();

	for (const auto& particle : particles) {
		particle->init();
	}

	IndexedFinder algo(effectLength);
	algo.add(particles);
	algo.createPairs(particles);
	auto& pairs = algo.getPairs();

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		pairs[i].solveDensity();
	}

	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		particles[i]->addSelfDensity();
	}

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		pairs[i].solvePressureForce();
		pairs[i].solveViscosityForce();
	}

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		pairs[i].getParticle1()->solveNormal(*pairs[i].getParticle2());
		pairs[i].getParticle2()->solveNormal(*pairs[i].getParticle1());

	}

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(pairs.size()); ++i) {
		pairs[i].solveSurfaceTension();
	}

	for (const auto& object : objects) {
		object->addExternalForce(externalForce);
	}

	BoundarySolver boundarySolver(timeStep, boundary);
	boundarySolver.solve(particles);

	for (const auto& object : objects) {
		object->addExternalForce(externalForce);
	}


	for (const auto& object : objects) {
		object->coordinate(timeStep);
	}

	for (const auto& object : objects) {
		object->forwardTime(timeStep);
	}
}

std::vector<SPHParticle*> PhysicsWorld::getParticles() const
{
	std::vector<SPHParticle*> results;
	for (const auto& object : objects) {
		const auto& particles = object->getParticles();
		results.insert(results.end(), particles.begin(), particles.end());
	}
	return results;
}