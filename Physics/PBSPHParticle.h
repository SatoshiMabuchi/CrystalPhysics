#ifndef __CRYSTAL_PHYSICS_PBSPH_PARTICLE_H__
#define __CRYSTAL_PHYSICS_PBSPH_PARTICLE_H__

#include <list>
#include <vector>
#include <memory>

#include "../../Crystal/Math/Vector3d.h"
#include "SPHKernel.h"

#include "../../Crystal/Util/UnCopyable.h"
#include "SPHConstant.h"

namespace Crystal {
	namespace Physics {

class PBSPHParticle : private UnCopyable
{
public:
	PBSPHParticle();

	PBSPHParticle(const Math::Vector3df& center, float radius, SPHConstant* constant);

	void setKernel(SPHKernel* kernel) { this->kernel = kernel; }

	virtual ~PBSPHParticle() {};

	void clearNeighbors() { neighbors.clear(); }

	void addNeighbor(PBSPHParticle* n) { neighbors.push_back(n); }

	void setNeighbors(const std::list<PBSPHParticle*>& neighbors);

	std::list<PBSPHParticle*> getNeighbors() const { return neighbors; }

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

	void addVelocity(const Math::Vector3df& velocity) { this->velocity += velocity; }

	void forwardTime(const float timeStep);

	void addExternalForce(const Math::Vector3df& force);

	void solveNormal(const PBSPHParticle& rhs);

	void solveSurfaceTension(const PBSPHParticle& rhs);

	void solvePressureForce(const PBSPHParticle& rhs);

	void addSelfDensity();

	void addDensity(const PBSPHParticle& rhs);

	void addDensity(const float distance, const float mass);

	void setNormal(const Math::Vector3df& n) { this->normal = n; }

	Math::Vector3df getNormal() const { return normal; }

	void predictPosition(const float dt);

	void solveConstrantGradient();

	void solveConstrantGradient(const PBSPHParticle& rhs);

	Math::Vector3df solveBoundaryDensityConstraint(const Math::Vector3df& pos);

	void solveDensityConstraint();

	void solveDensity();

	void updatePredictPosition(const float dt);

	void updateVelocity(const float dt);

	void updatePosition();

	void solvePositionCorrection();

	//void addPositionCorrection(const Math::Vector3d<float>& pc) { this->positionCorrection += pc; }
	void addPositionCorrection(const Math::Vector3df& pc);

	void addConstrantGradient(const Math::Vector3df& distance);

	void solveViscosity();

	void solveViscosity(const float distance);

	void updateViscosity() { this->velocity += this->viscVelocity; }

	float getEffectLength() const { return constant->getEffectLength(); }

	Math::Vector3df getPosition() const { return position; }

	float getDiameter() const { return radius * 2.0f; }

private:
	Math::Vector3df getConstraintGradient(const PBSPHParticle& rhs);

	Math::Vector3df getPositionCorrection(const PBSPHParticle& rhs);

	Math::Vector3df getDiff(const PBSPHParticle& rhs) const;

	Math::Vector3df solveViscosity(const PBSPHParticle& rhs);

	float getDensityConstraintCorrection(const PBSPHParticle& rhs) const;

	float densityConstraint;

	std::list<PBSPHParticle*> neighbors;
	float radius;
	float density;
	Math::Vector3df position;
	Math::Vector3df force;
	Math::Vector3df velocity;
	Math::Vector3df normal;
	Math::Vector3df prevPosition;
	Math::Vector3df constraintGrad;

	SPHConstant* constant;
	Math::Vector3df positionCorrection;
	Math::Vector3df viscVelocity;
	SPHKernel* kernel;
};

	}
}

#endif