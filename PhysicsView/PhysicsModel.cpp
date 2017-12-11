#include "PhysicsModel.h"
#include "../Physics/SPHParticle.h"
#include "../Physics/STSPHSolver.h"

using namespace Crystal::Math;
using namespace Crystal::Graphics;
using namespace Crystal::UI;
using namespace Crystal::Physics;

PhysicsModel::PhysicsModel()
{
	solver = new STSPHSolver();
	solver->setExternalForce(Vector3df(0.0, -9.8, 0.0));
	solver->setBoundary(Box3d(Vector3df(-100.0, 0.0, -10.0), Vector3df(100, 100.0, 10.0)));
}

void PhysicsModel::clear()
{
	//solver->getParticles();
}

void PhysicsModel::addParticle(SPHParticle* particle) {
	solver->add(particle);
}

ViewModel PhysicsModel::toViewModel() const
{
	ViewModel vm;
	const auto& particles = solver->getParticles();
	for (auto p : particles) {
		vm.add(p->getPosition(), ColorRGBAf(1,1,1,1), 1000.0f);
	}
	return vm;
}

Box3d PhysicsModel::getBoundingBox() const
{
	const auto& particles = solver->getParticles();
	if (particles.empty()) {
		return Box3d();
	}
	Box3d bb(particles.front()->getPosition());
	for (auto p : particles) {
		bb.add(p->getPosition());
	}
	return bb;
}