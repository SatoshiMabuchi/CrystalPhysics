#pragma once

#include <vector>
#include "../../Crystal/Math/Box3d.h"

namespace Crystal {
	namespace Physics {

class ISPHSolver
{
public:
	void setBoundary(const Math::Box3d& boundary) { this->boundary = boundary; }

	void setExternalForce(const Math::Vector3df& force) { this->externalForce = force; }

	virtual void simulate(const float effectLength, const float timeStep) = 0;

protected:
	Math::Vector3df externalForce;
	Math::Box3d boundary;


};
	}
}

