#include "PBSPHParticle.h"

#include "SPHKernel.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;


PBSPHParticle::PBSPHParticle(const Vector3df& center, float radius, SPHConstant* constant) :
	ISPHParticle(center, radius),
	constant(constant)
{
	this->density = constant->getDensity();
}

void PBSPHParticle::setNeighbors(const std::list<PBSPHParticle*>& neighbors)
{
	this->neighbors = neighbors;
	this->neighbors.remove(this);
}

void PBSPHParticle::init()
{
	neighbors.clear();
	density = 0.0;
	normal = Math::Vector3df(0.0f, 0.0f, 0.0f);
	force = Math::Vector3df(0.0f, 0.0f, 0.0f);
}

float PBSPHParticle::getDensityRatio() const
{
	return density / constant->getDensity();
}

float PBSPHParticle::getMass() const
{
	return constant->getDensity() * std::pow(getDiameter(), 3);
}

float PBSPHParticle::getVolume() const
{
	return getMass() / density;
}

float PBSPHParticle::getRestVolume() const
{
	return getMass() / constant->getDensity();
}

void PBSPHParticle::forwardTime(const float timeStep)
{
	const auto& acc = getAccelaration();
	this->velocity += (acc* timeStep);
	this->position += (this->velocity * timeStep);
}

void PBSPHParticle::addExternalForce(const Vector3df& externalForce)
{
	this->force += externalForce * getMass();//constant->getDensity();
}

void PBSPHParticle::solveViscosity()
{
	viscVelocity = Vector3df(0, 0, 0);
	const auto scale = 0.001f;
	for (auto n : neighbors) {
		viscVelocity += scale * solveViscosity(*n);
	}
	//this->velocity += viscVelocity;
}


void PBSPHParticle::addSelfDensity()
{
	this->addDensity(kernel->getPoly6Kernel2(0.0) * this->getMass());
}

void PBSPHParticle::addDensity(const PBSPHParticle& rhs)
{
	const float distanceSquared = ::getDistanceSquared( this->getPosition(), rhs.getPosition());
	this->addDensity(kernel->getPoly6Kernel2(distanceSquared) * rhs.getMass());
}

void PBSPHParticle::addDensity(const float distance, const float mass)
{
	this->addDensity(kernel->getPoly6Kernel(distance) * mass);
}


void PBSPHParticle::predictPosition(const float dt)
{
	this->prevPosition = this->position;
	this->velocity += dt * this->force;
	this->position += dt * this->velocity;
}

void PBSPHParticle::solveConstrantGradient()
{
	this->constraintGrad = Vector3df(0, 0, 0);
	for (auto n : neighbors) {
		this->constraintGrad += this->getConstraintGradient(*n);
	}
}

void PBSPHParticle::solveConstrantGradient(const PBSPHParticle& rhs)
{
	this->constraintGrad += this->getConstraintGradient(rhs);
}

void PBSPHParticle::addConstrantGradient(const Vector3df& distanceVector)
{
	if (glm::length(distanceVector) > 1.0e-3) {
		this->constraintGrad +=
			getMass() * 1.0f / this->constant->getDensity() * kernel->getSpikyKernelGradient(distanceVector);
	}
}

Vector3df PBSPHParticle::getConstraintGradient(const PBSPHParticle& rhs)
{
	const auto& distanceVector = getDiff(rhs);
	return getMass() * 1.0f / this->constant->getDensity() * kernel->getSpikyKernelGradient(distanceVector);
}

Vector3df PBSPHParticle::solveBoundaryDensityConstraint(const Vector3df& pos)
{
	return getMass() * 1.0f / this->constant->getDensity() * kernel->getSpikyKernelGradient(pos - this->getPosition());
}


void PBSPHParticle::solveDensityConstraint()
{
	this->densityConstraint = 0.0f;
	const auto c = std::max(0.0f, this->getDensity() / this->constant->getDensity() - 1.0f);
	auto sum = 0.0f;
	for (auto n : neighbors) {
		if (getDistanceSquared( n->getPosition(), this->getPosition()) < (this->constant->getEffectLength() * this->constant->getEffectLength())) {
			sum += getLengthSquared( n->constraintGrad );
		}
	}
	//auto sum = this->constraintGrad.getLengthSquared();
	sum += 1.0e-3f;
	this->densityConstraint = -c / sum;
}

void PBSPHParticle::solvePositionCorrection()
{
	this->positionCorrection = Vector3df(0, 0, 0);
	for (auto n : neighbors) {
		this->positionCorrection += getPositionCorrection(*n);
	}
}


Vector3df PBSPHParticle::getPositionCorrection(const PBSPHParticle& rhs)
{
	const auto& distanceVector = getDiff(rhs);
	const auto densityCorrection = 0.0f;//getDensityConstraintCorrection(rhs);
	return getMass() * 1.0f / this->constant->getDensity() * (this->densityConstraint + rhs.densityConstraint + densityCorrection) * kernel->getSpikyKernelGradient(distanceVector);
}

void PBSPHParticle::solveDensity()
{
	this->density = 0.0f;
	for (auto n : neighbors) {
		addDensity(*n);
	}
	addSelfDensity();
}

Vector3df PBSPHParticle::solveViscosity(const PBSPHParticle& rhs)
{
	const auto& velocityDiff = (rhs.velocity - this->velocity);
	const auto distance = glm::distance( getPosition(), rhs.getPosition());
	const auto weight = kernel->getPoly6Kernel(distance);
	return velocityDiff * weight;
}

void PBSPHParticle::solveViscosity(const float distance)
{
	const auto& velocityDiff = (-this->velocity);
	const auto weight = kernel->getPoly6Kernel(distance);
	this->viscVelocity += 0.1f * velocityDiff * weight;
}

void PBSPHParticle::updatePredictPosition(const float dt)
{
	this->position = this->position + positionCorrection;
}

void PBSPHParticle::updateVelocity(const float dt)
{
	this->velocity = (this->position - this->prevPosition) / dt;
}

void PBSPHParticle::updatePosition()
{
	this->prevPosition = this->position;
}

void PBSPHParticle::addPositionCorrection(const Vector3df& distanceVector)
{
	this->positionCorrection += distanceVector;
	//const auto pc = 1.0f / this->constant->getDensity() * (this->densityConstraint + this->densityConstraint) * kernel.getSpikyKernelGradient(distanceVector, constant->getEffectLength());
	//this->positionCorrection += pc;
}

Vector3df PBSPHParticle::getDiff(const PBSPHParticle& rhs) const
{
	//return this->getPosition() - rhs.getPosition();
	return rhs.getPosition() - this->getPosition();
}

float PBSPHParticle::getDensityConstraintCorrection(const PBSPHParticle& rhs) const
{
	const float k = 0.1f;
	const float n = 4;
	const float dq = 0.1f * constant->getEffectLength();
	const auto w1 = kernel->getPoly6Kernel(glm::length( getDiff(rhs) ));
	const auto w2 = kernel->getPoly6Kernel(dq);
	return -k * std::pow(w1 / w2, n);
}

void PBSPHParticle::solveNormal(const PBSPHParticle& rhs)
{
	const auto& distanceVector = this->getPosition() - rhs.getPosition();
	this->normal += kernel->getPoly6KernelGradient(distanceVector, constant->getEffectLength()) * rhs.getVolume();
	//pairs[i].getParticle1()->addForce(viscosityCoe * velocityDiff * kernel.getViscosityKernelLaplacian(distance, effectLength) * pairs[i].getParticle2()->getVolume());
}