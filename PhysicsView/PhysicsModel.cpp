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
	physicsObjects.clear();
}

void PhysicsModel::addPhysicsObject(PhysicsObject* object) {
	solver->add(object);
	physicsObjects.push_back(object);
}

ViewModel PhysicsModel::toViewModel() const
{
	ViewModel vm;
	const auto& particleSystems = physicsObjects;
	for (const auto& ps : particleSystems) {
		const auto& particles = ps->getParticles();
		for (auto p : particles) {
			vm.add(p->getPosition(), ColorRGBAf(1,1,1,1), 1000.0f);
		}
	}
	return vm;
}

Box3d PhysicsModel::getBoundingBox() const
{
	if (physicsObjects.empty()) {
		return Box3d();
	}
	Box3d bb(physicsObjects.front()->getBoundingBox());
	for (auto ps : physicsObjects) {
		bb.add( ps->getBoundingBox() );
	}
	return bb;
}