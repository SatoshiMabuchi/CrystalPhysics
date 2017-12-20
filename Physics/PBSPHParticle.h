#ifndef __CRYSTAL_PHYSICS_PBSPH_PARTICLE_H__
#define __CRYSTAL_PHYSICS_PBSPH_PARTICLE_H__

#include <list>
#include <vector>
#include <memory>

#include "../../Crystal/Math/Vector3d.h"
#include "SPHKernel.h"
#include "ISPHParticle.h"
#include "SPHConstant.h"

namespace Crystal {
	namespace Physics {

class PBSPHParticle : public ISPHParticle
{
public:
	PBSPHParticle(const Math::Vector3df& center, float radius, SPHConstant* constant);

	void setKernel(SPHKernel* kernel) { this->kernel = kernel; }

	virtual ~PBSPHParticle() {};

	float getDensityRatio() const;

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

	void addVelocity(const Math::Vector3df& velocity) {
		this->velocity += velocity;
		this->velocity *= 0.99;
	}

	void forwardTime(const float timeStep);

	void addExternalForce(const Math::Vector3df& force);

	void addSelfDensity();

	void addDensity(const PBSPHParticle& rhs);

	void predictPosition_(const float dt);

	void updatePredictPosition();

	void updateVelocity(const float dt);

	void updatePosition();

	void addPositionCorrection(const Math::Vector3df& pc);

	void solveViscosity(const float distance);

	float getEffectLength() const { return constant->getEffectLength(); }

	Math::Vector3df getPredictPosition() const { return predictPosition; }

	Math::Vector3df getPosition() const { return position; }

	void calculatePressure(const PBSPHParticle& rhs);

	void calculateViscosity(const PBSPHParticle& rhs);

	float getConstraint() const;

	Math::Vector3df dx;
	Math::Vector3df xvisc;

	void setDensity(const float d) { this->density = d; }


private:
	Math::Vector3df getDiff(const PBSPHParticle& rhs) const;

	Math::Vector3df predictPosition;
	Math::Vector3df force;
	Math::Vector3df velocity;
	Math::Vector3df normal;

	SPHConstant* constant;
	SPHKernel* kernel;
};

	}
}

#endif