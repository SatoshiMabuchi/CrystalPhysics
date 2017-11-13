#pragma once

#include <vector>
#include "PhysicsObject.h"
#include "ISPHSolver.h"

namespace Crystal {
	namespace Physics {

class DFSPHSolver : public ISPHSolver
{
public:
	virtual void simulate(const float effectLength, const float timeStep);

	std::vector< PhysicsObject* > objects;

private:
	void calculateFactor();
};
	}
}