#pragma once

#include <vector>
#include "PhysicsObject.h"

namespace Crystal {
	namespace Physics {

class ISPHSolver
{
public:
	void add(PhysicsObject* object) { objects.push_back(object); }

	std::vector< SPHParticle*> getParticles() const;

	void setBoundary(const Math::Box3d& boundary) { this->boundary = boundary; }

	void setExternalForce(const Math::Vector3df& force) { this->externalForce = force; }

	virtual void simulate(const float effectLength, const float timeStep) = 0;

protected:
	std::vector< PhysicsObject* > objects;
	Math::Vector3df externalForce;
	Math::Box3d boundary;


};
	}
}

