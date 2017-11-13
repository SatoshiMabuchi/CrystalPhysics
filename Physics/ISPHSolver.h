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

protected:
	std::vector< PhysicsObject* > objects;

};
	}
}

