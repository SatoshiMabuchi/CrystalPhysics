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
	SPHParticle(const Math::Vector3d<float>& center, const float radius, SPHConstant* constant);

	virtual ~SPHParticle() {};

	float getDensityRatio() const;

	float getPressure() const;

	float getMass() const;

	float getVolume() const;

	float getRestVolume() const;

	void addForce(const Math::Vector3d<float>& force) { this->force += force; }

	void setForce(const Math::Vector3d<float>& force) { this->force = force; }

	Math::Vector3d<float> getForce() const { return force; }

	void setDefaultDensity() { this->density = constant->getDensity(); }

	float getDensity() const { return density; }

	void addDensity(const float density) { this->density += density; }

	void init();

	Math::Vector3d<float> getAccelaration() { return force / density; }

	Math::Vector3d<float> getVelocity() const { return velocity; }

	void setVelocity(const Math::Vector3d<float>& velocity) { this->velocity = velocity; }

	void addVelocity(const Math::Vector3d<float>& velocity) { this->velocity += velocity; }

	void forwardTime(const float timeStep);

	void addExternalForce(const Math::Vector3d<float>& force);

	void solveNormal(const SPHParticle& rhs);

	void solveSurfaceTension(const SPHParticle& rhs);

	void solvePressureForce(const SPHParticle& rhs);

	void solveViscosityForce(const SPHParticle& rhs);

	void addSelfDensity();

	void addDensity(const SPHParticle& rhs);

	Math::Vector3d<float> getNormal() const { return normal; }

	bool isBoundary() const { return constant->isBoundary; }

	Math::Vector3d<float> getPosition() const { return position; }

	float getDiameter() const { return radius * 2.0f; }

	void move(const Math::Vector3d<float>& v);

private:
	Math::Vector3d<float> position;
	Math::Vector3d<float> force;
	Math::Vector3d<float> velocity;
	Math::Vector3d<float> normal;
	float density;
	float radius;
	SPHConstant* constant;
};

	}
}

#endif