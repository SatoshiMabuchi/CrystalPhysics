#ifndef __CRYSTAL_PHYSICS_SPH_PARTICLE_H__
#define __CRYSTAL_PHYSICS_SPH_PARTICLE_H__

#include <list>
#include <vector>
#include <memory>

#include "../../Crystal/Math/Vector3d.h"
#include "../../Crystal/Util/UnCopyable.h"
#include "SPHConstant.h"

namespace Crystal {
	namespace Physics {

class SPHParticle : private UnCopyable
{
public:
	SPHParticle(const Math::Vector3df& center, const float radius, SPHConstant* constant);

	virtual ~SPHParticle() {};

	float getDensityRatio() const;

	float getPressure() const;

	float getMass() const;

	float getVolume() const;

	float getRestVolume() const;

	void addForce(const Math::Vector3df& force) { this->force += force; }

	void setForce(const Math::Vector3df& force) { this->force = force; }

	Math::Vector3df getForce() const { return force; }

	void setDefaultDensity() { this->density = constant->getDensity(); }

	float getDensity() const { return density; }

	void addDensity(const float density) { this->density += density; }

	void init();

	Math::Vector3df getAccelaration() { return force / density; }

	Math::Vector3df getVelocity() const { return velocity; }

	void setVelocity(const Math::Vector3df& velocity) { this->velocity = velocity; }

	void addVelocity(const Math::Vector3df& velocity) { this->velocity += velocity; }

	void forwardTime(const float timeStep);

	void addExternalForce(const Math::Vector3df& force);

	void solveNormal(const SPHParticle& rhs);

	void solveSurfaceTension(const SPHParticle& rhs);

	void solvePressureForce(const SPHParticle& rhs);

	void solveViscosityForce(const SPHParticle& rhs);

	void addSelfDensity();

	void addDensity(const SPHParticle& rhs);

	Math::Vector3df getNormal() const { return normal; }

	bool isBoundary() const { return constant->isBoundary; }

	Math::Vector3df getPosition() const { return position; }

	float getDiameter() const { return radius * 2.0f; }

	void move(const Math::Vector3df& v);

private:
	Math::Vector3df position;
	Math::Vector3df force;
	Math::Vector3df velocity;
	Math::Vector3df normal;
	float density;
	float radius;
	SPHConstant* constant;
};

	}
}

#endif