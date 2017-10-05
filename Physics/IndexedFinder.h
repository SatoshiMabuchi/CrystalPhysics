#ifndef __CRYSTAL_PHYSICS_INDEXED_FINDER_H__
#define __CRYSTAL_PHYSICS_INDEXED_FINDER_H__

#include "../../Crystal/Util/UnCopyable.h"

#include "IndexedParticle.h"
#include <vector>
#include "ParticlePair.h"

namespace Crystal {
	namespace Physics {

		class IndexedFinder final : private UnCopyable {
		public:
			IndexedFinder(const float effectLength) :
				effectLength(effectLength)
			{}

			void add(const std::vector<IndexedParticle>& particles);

			void add(const std::vector<SPHParticle*>& particles);

			void createPairs(std::vector<SPHParticle*> particles);

			std::vector<ParticlePair> getPairs() const { return pairs; }

			//std::list<SPHParticle*> find(const Math::Vector3d<float>& position);

		private:
			std::vector<ParticlePair> pairs;

			std::vector<ParticlePair> search1(const std::vector<IndexedParticle>& particles, std::vector<IndexedParticle>::const_iterator startIter, std::vector<IndexedParticle>::const_iterator endIter, const float effectLengthSquared);

			std::vector<ParticlePair> search2(const std::vector<IndexedParticle>& particles, std::vector<IndexedParticle>::const_iterator startIter, std::vector<IndexedParticle>::const_iterator endIter, const float effectLengthSquared);

			std::vector<IndexedParticle> iparticles;
			//std::vector<IndexedParticle> iparticlesy;
			//std::vector<IndexedParticle> iparticlesz;
			const float effectLength;
		};

	}
}

#endif