#include "BoundarySolver.h"

#include "SPHParticle.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

BoundarySolver::BoundarySolver(const float timeStep, const Box3d<float>& boundary) :
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

Vector3d<float> BoundarySolver::getBoundaryForce(const Vector3d<float>& center)
{
	Math::Vector3d<float> force(0,0,0);

	force += getForceX(center.getX());
	force += getForceY(center.getY());
	force += getForceZ(center.getZ());

	return force;
}

Vector3d<float> BoundarySolver::getForceX(const float x)
{
	float over = 0;
	if (x > boundary.getMaxX()) {
		over = x - boundary.getMaxX();
	}
	else if (x < boundary.getMinX()) {
		over = x - boundary.getMinX();
	}

	const float force = getForce(over);
	return Vector3d<float>(1,0,0) * force;
}

Vector3d<float> BoundarySolver::getForceY(const float y)
{
	float over = 0;
	if (y > boundary.getMaxY()) {
		over = y - boundary.getMaxY();
	}
	else if (y < boundary.getMinY()) {
		over = y - boundary.getMinY();
	}
	const float force = getForce(over);
	return Vector3d<float>(0,1,0) * force;
}

Vector3d<float> BoundarySolver::getForceZ(const float z)
{
	float over = 0;
	if (z > boundary.getMaxZ()) {
		over = z - boundary.getMaxZ();
	}
	else if (z < boundary.getMinZ()) {
		over = z - boundary.getMinZ();
	}
	const float force = getForce(over);
	return Vector3d<float>(0,0,1) * force;
}

float BoundarySolver::getForce(const float over) {
	return -over / timeStep / timeStep;
}