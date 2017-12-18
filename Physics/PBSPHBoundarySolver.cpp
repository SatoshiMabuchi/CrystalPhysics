#include "PBSPHBoundarySolver.h"

#include "PBSPHParticle.h"
#include "SPHKernel.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

namespace {
	SPHKernel kernel;
}

PBSPHBoundarySolver::PBSPHBoundarySolver(const Box3d& boundary) :
	boundary(boundary)
{
}

PBSPHParticle* PBSPHBoundarySolver::generateBoundaryParticle(PBSPHParticle* particle)
{
	const auto& bPos = getBoundaryPosition(particle);
	auto p = new PBSPHParticle(bPos, particle->getRadius(), particle->getConstant());
	p->init();
	p->setBoundary();
	return p;
}

void PBSPHBoundarySolver::solveDensity(const std::vector<PBSPHParticle*>& particles)
{
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		if (isBoundary(particles[i])) {
			const auto& bPos = getBoundaryPosition(particles[i]);
			const auto dist = glm::distance( particles[i]->getPosition(), bPos);
			particles[i]->addDensity(dist, particles[i]->getMass());
		}
	}
}

void PBSPHBoundarySolver::solveConstraintGradient(const std::vector<PBSPHParticle*>& particles)
{
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		if (isBoundary(particles[i])) {
			const auto& bPos = getBoundaryPosition(particles[i]);
			const auto v = bPos - particles[i]->getPosition();
			particles[i]->addConstrantGradient(v);
		}
	}
}

/*
void PBSPHBoundarySolver::solveDensityConstraint(const std::vector<PBSPHParticle*>& particles)
{
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		if (isBoundary(particles[i])) {
			const auto& bPos = getBoundaryPosition(particles[i]);
			const auto v = bPos - particles[i]->getPosition();
			particles[i]->solveDensityConstraint(v);
		}
	}
}
*/

void PBSPHBoundarySolver::solveViscosity(const std::vector<PBSPHParticle*>& particles)
{
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		const auto v = getOverVector(particles[i]->getPosition());
		particles[i]->solveViscosity(glm::length( v ));
	}
}


void PBSPHBoundarySolver::solveCorrectPosition(const std::vector<PBSPHParticle*>& particles)
{
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		if (isBoundary(particles[i])) {
			const auto& bPos = getBoundaryPosition(particles[i]);
			const auto v = bPos - particles[i]->getPosition();
			particles[i]->addPositionCorrection(v);
		}
	}
}

void PBSPHBoundarySolver::solveForce(const std::vector<PBSPHParticle*>& particles, const float dt)
{
	for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
		const auto over = getOverVector(particles[i]->getPosition());
		particles[i]->addExternalForce(-over / dt / dt);
	}
}

bool PBSPHBoundarySolver::isBoundary(PBSPHParticle* particle)
{
	const auto& pos = particle->getPosition();
	const auto radii = particle->getRadius();
	if (pos.x + radii > boundary.getMaxX()) {
		return true;
	}
	if (pos.y + radii > boundary.getMaxY()) {
		return true;
	}
	if (pos.z + radii > boundary.getMaxZ()) {
		return true;
	}
	if (pos.x - radii < boundary.getMinX()) {
		return true;
	}
	if (pos.y - radii < boundary.getMinY()) {
		return true;
	}
	if (pos.z - radii < boundary.getMinZ()) {
		return true;
	}
	return false;
}

Vector3df PBSPHBoundarySolver::getBoundaryPosition(PBSPHParticle* particle)
{
	assert(isBoundary(particle));
	const auto& pos = particle->getPosition();
	const auto radii = particle->getRadius();
	if (pos.x + radii > boundary.getMaxX()) {
		return Vector3df(boundary.getMaxX() + radii, pos.y, pos.z);
	}
	if (pos.y + radii > boundary.getMaxY()) {
		return Vector3df(pos.x, boundary.getMaxY() + radii, pos.z);
	}
	if (pos.z + radii > boundary.getMaxZ()) {
		return Vector3df(pos.x, pos.y, boundary.getMaxZ() + radii);
	}
	if (pos.x - radii < boundary.getMinX()) {
		return Vector3df(boundary.getMinX() - radii, pos.y, pos.z);
	}
	if (pos.y - radii < boundary.getMinY()) {
		return Vector3df(pos.x, boundary.getMinY() - radii, pos.z);
	}
	if (pos.z - radii < boundary.getMinZ()) {
		return Vector3df(pos.x, pos.y, boundary.getMinZ() - radii);
	}
	assert(false);
	return Vector3df(0, 0, 0);
}


Vector3df PBSPHBoundarySolver::getOverVector(const Vector3df& position)
{
	return Vector3df(getOverX(position.x), getOverY(position.y), getOverZ(position.z));
}

float PBSPHBoundarySolver::getOverX(const float x)
{
	float over = 0;
	if (x > boundary.getMaxX()) {
		over = x - boundary.getMaxX();
	}
	else if (x < boundary.getMinX()) {
		over = x - boundary.getMinX();
	}
	return over;
}

float PBSPHBoundarySolver::getOverY(const float y)
{
	float over = 0;
	if (y > boundary.getMaxY()) {
		over = y - boundary.getMaxY();
	}
	else if (y < boundary.getMinY()) {
		over = y - boundary.getMinY();
	}
	return over;
}

float PBSPHBoundarySolver::getOverZ(const float z)
{
	float over = 0;
	if (z > boundary.getMaxZ()) {
		over = z - boundary.getMaxZ();
	}
	else if (z < boundary.getMinZ()) {
		over = z - boundary.getMinZ();
	}
	return over;
}
