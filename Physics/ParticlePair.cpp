#include "ParticlePair.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

ParticlePair::ParticlePair() :
	particle1(nullptr),
	particle2(nullptr)
{}

ParticlePair::ParticlePair(SPHParticle* particle1, SPHParticle* particle2) :
	particle1(particle1),
	particle2(particle2)
{
}

Vector3df ParticlePair::getDistanceVector() const
{
	return Math::Vector3df(particle1->getPosition() - particle2->getPosition());
}

float ParticlePair::getDistance() const
{
	return glm::length( getDistanceVector() );
}

float ParticlePair::getDistanceSquared() const
{
	return Math::getDistanceSquared( particle1->getPosition(), particle2->getPosition());
}