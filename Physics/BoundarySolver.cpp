#include "BoundarySolver.h"

#include "SPHParticle.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

BoundarySolver::BoundarySolver(const float timeStep, const Box3d& boundary) :
	timeStep(timeStep),
	boundary(boundary)
{
}


void BoundarySolver::solve(const std::vector<SPHParticle*>& particles) {
#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		const auto force = getBoundaryForce(particles[i]->getPosition());
		particles[i]->addForce(force * particles[i]->getDensity());
	}
}

Vector3df BoundarySolver::getBoundaryForce(const Vector3df& center)
{
	Math::Vector3df force(0,0,0);

	force += getForceX(center.x);
	force += getForceY(center.y);
	force += getForceZ(center.z);

	return force;
}

Vector3df BoundarySolver::getForceX(const float x)
{
	float over = 0;
	if (x > boundary.getMaxX()) {
		over = x - boundary.getMaxX();
	}
	else if (x < boundary.getMinX()) {
		over = x - boundary.getMinX();
	}

	const float force = getForce(over);
	return Vector3df(1,0,0) * force;
}

Vector3df BoundarySolver::getForceY(const float y)
{
	float over = 0;
	if (y > boundary.getMaxY()) {
		over = y - boundary.getMaxY();
	}
	else if (y < boundary.getMinY()) {
		over = y - boundary.getMinY();
	}
	const float force = getForce(over);
	return Vector3df(0,1,0) * force;
}

Vector3df BoundarySolver::getForceZ(const float z)
{
	float over = 0;
	if (z > boundary.getMaxZ()) {
		over = z - boundary.getMaxZ();
	}
	else if (z < boundary.getMinZ()) {
		over = z - boundary.getMinZ();
	}
	const float force = getForce(over);
	return Vector3df(0,0,1) * force;
}

float BoundarySolver::getForce(const float over)
{
	return -over / timeStep / timeStep;
}