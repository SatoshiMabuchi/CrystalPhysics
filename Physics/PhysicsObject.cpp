#include "PhysicsObject.h"
#include "SPHParticle.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

PhysicsObject::PhysicsObject(const std::vector<SPHParticle*>& particles) :
	particles(particles),
	nextId(0)
{
}

/*

PhysicsObject::PhysicsObject(const Box3d& box, const float divideLength, const SPHConstant& constant) :
	constant(constant),
	nextId(0)
{
	const auto points = box.toPoints(divideLength);
	for (const auto& pos : points) {
		SPHParticle* p = new SPHParticle(pos, divideLength*0.5f, &this->constant);
		particles.push_back(p);
	}
}
*/


/*
PhysicsObject::PhysicsObject(const Sphere<float>& sphere, const float divideLength, const SPHConstant& constant) :
	nextId(0)
{
	const auto points = sphere.toPoints(divideLength);
	for (const auto& pos : points) {
		SPHParticle* p = new SPHParticle(pos, divideLength*0.5f, &this->constant, nextId++);
		particles.push_back(p);
	}
}
*/

PhysicsObject::~PhysicsObject()
{
	clear();
}

SPHParticle* PhysicsObject::createParticle(const Vector3df& position, const Vector3df& velocity)
{
	auto p = new SPHParticle(position, constant.getEffectLength() / 1.25f / 2.0f, &constant);
	p->setVelocity(velocity);
	particles.push_back(p);
	return p;
}


/*
std::vector<SPHParticle*> PhysicsObject::createParticles(const Box3d& box, const float divideLength)
{
	std::vector<SPHParticle*> newParticles;
	const auto points = box.toPoints(divideLength);
	for (const auto& pos : points) {
		SPHParticle* p = new SPHParticle(pos, divideLength*0.5f, &this->constant);
		particles.push_back(p);
		newParticles.push_back(p);
	}
	return newParticles;
}
*/
/*
std::vector<SPHParticle*> PhysicsObject::createParticles(const Sphere<float>& sphere, const float divideLength)
{
	std::vector<SPHParticle*> newParticles;

	const auto points = sphere.toPoints(divideLength);
	for (const auto& pos : points) {
		SPHParticle* p = new SPHParticle(pos, divideLength*0.5f, &this->constant, nextId++);
		particles.push_back(p);
		newParticles.push_back(p);
	}
	return newParticles;
}
*/

float PhysicsObject::getMass() const
{
	auto weight = 0.0f;
	for (const auto& particle : particles) {
		weight += particle->getMass();
	}
	return weight;
}

float PhysicsObject::getRestVolume() const
{
	auto volume = 0.0f;
	for (const auto& particle : particles) {
		volume += particle->getRestVolume();
	}
	return volume;
}

void PhysicsObject::forwardTime(const float timeStep)
{
	for (const auto& p : particles) {
		p->forwardTime(timeStep);
	}
}

void PhysicsObject::addExternalForce(const Vector3df& externalForce)
{
	for (const auto& p : particles) {
		p->addExternalForce(externalForce);
	}
}

Vector3df PhysicsObject::getCenter() const
{
	if (particles.empty()) {
		return Math::Vector3df(0.0, 0.0, 0.0);
	}
	Math::Vector3df center(0.0, 0.0, 0.0);
	for (const auto& p : particles) {
		center += p->getPosition();
	}
	return center /= static_cast<float>(particles.size());
}

Vector3df PhysicsObject::getAverageVelosity() const
{
	Math::Vector3df averageVelosity(0.0, 0.0, 0.0);
	for (const auto& p : particles) {
		averageVelosity += p->getVelocity();// variable.velocity;
	}
	return averageVelosity / static_cast<float>(particles.size());
}

float PhysicsObject::getWeight() const
{
	auto weight = 0.0f;
	for (const auto& particle : particles) {
		weight += particle->getMass();
	}
	return weight;
}

void PhysicsObject::convertToFluidForce()
{
	Math::Vector3df totalForce(0.0, 0.0, 0.0);
	for (const auto& p : particles) {
		totalForce += p->getForce() * p->getVolume();
	}

	const float weight = getWeight();
	for (const auto& p : particles) {
		p->setForce(totalForce / weight * p->getDensity());
	}
}

void PhysicsObject::move(const Vector3df& v)
{
	for (const auto& p : particles) {
		p->move(v);
	}
}

void PhysicsObject::setVelocity(const Vector3df& velocity)
{
	for (const auto& p : particles) {
		p->setVelocity(velocity);
	}
}

Box3d PhysicsObject::getBoundingBox() const
{
	if (particles.empty()) {
		return Box3d();
	}
	Box3d box(particles.front()->getPosition());
	for (auto p : particles) {
		box.add( p->getPosition() );
	}
	return box;
}
