#ifndef __CRYSTAL_PHYSICS_PARTICLE_PAIR_H__
#define __CRYSTAL_PHYSICS_PARTICLE_PAIR_H__

#include "SPHParticle.h"

namespace Crystal {
	namespace Physics {

class ParticlePair
{
public:
	ParticlePair();

	ParticlePair(SPHParticle* particle1, SPHParticle* particle2);

	Math::Vector3df getDistanceVector() const;

	float getDistance() const;

	float getDistanceSquared() const;

	SPHParticle* getParticle1() const { return particle1; }

	SPHParticle* getParticle2() const { return particle2; }

private:
	SPHParticle* particle1;
	SPHParticle* particle2;
};

	}
}

#endif