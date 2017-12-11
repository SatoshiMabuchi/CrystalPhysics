#pragma once

#include <vector>
#include "ISPHSolver.h"

namespace Crystal {
	namespace Physics {

class DFSPHSolver : public ISPHSolver
{
public:
	virtual void simulate(const float effectLength, const float timeStep);

private:
	void calculateFactor();
};
	}
}