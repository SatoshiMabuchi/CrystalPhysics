#pragma once

#include <vector>
#include "PhysicsObject.h"
#include "ISPHSolver.h"

namespace Crystal {
	namespace Physics {

class DFSPHSolver : public ISPHSolver
{
public:
	void simulate(const double dt, const double effectLength);

	std::vector< PhysicsObject* > objects;

private:
	void calculateFactor();
};
	}
}