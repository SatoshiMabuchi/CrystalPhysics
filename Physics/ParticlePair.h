#ifndef __CRYSTAL_PHYSICS_PARTICLE_PAIR_H__
#define __CRYSTAL_PHYSICS_PARTICLE_PAIR_H__

#include "SPHParticle.h"

namespace Crystal {
	namespace Physics {

class ParticlePair
{
public:
	ParticlePair();

	ParticlePair(ISPHParticle* particle1, ISPHParticle* particle2);

	Math::Vector3df getDistanceVector() const;

	float getDistance() const;

	float getDistanceSquared() const;

	ISPHParticle* getParticle1() const { return particle1; }

	ISPHParticle* getParticle2() const { return particle2; }

private:
	ISPHParticle* particle1;
	ISPHParticle* particle2;
};

	}
}

#endif