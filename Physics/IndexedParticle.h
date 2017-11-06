#ifndef __CRYSTAL_PHYSICS_INDEXED_PARTICLE_H__
#define __CRYSTAL_PHYSICS_INDEXED_PARTICLE_H__

#include "../../Crystal/Math/Vector3d.h"
#include "SPHParticle.h"

namespace Crystal {
	namespace Physics {

class IndexedParticle
{
public:
	IndexedParticle() :
		particle(nullptr)
	{}

	IndexedParticle(SPHParticle* particle) :
		particle(particle)
	{
	}

public:
	void setGridID(const float effectLength);

	Math::Vector3df getPosition() const { return particle->getPosition(); }

	static int toGridId(const Math::Vector3df pos, const float effectLength);

	static std::array<int,3> toIndex(const Math::Vector3df pos, const float effectLength);

	int getGridID() const { return gridID; }

	bool operator<(const IndexedParticle& rhs) {
		return this->getGridID() < rhs.getGridID();
	}

	SPHParticle* getParticle() const { return particle; }

private:
	static int toIdX(std::array<int, 3> index);

private:
	SPHParticle* particle;
	int gridID;
};

	}
}

#endif