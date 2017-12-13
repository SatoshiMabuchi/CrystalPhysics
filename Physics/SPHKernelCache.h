#pragma once

#include <array>
#include "../../Crystal/Math/Vector3d.h"

namespace Crystal {
	namespace Physics {

class SPHKernelCache
{
public:
	static SPHKernelCache* getInstance() {
		static SPHKernelCache cache;
		return &cache;
	}

	void build(const float effectLength);

	float getCubicSpline(const float v) const;

	Math::Vector3df getSpikyGradient(const Math::Vector3df& v) const;

private:
	std::array<float, 200> cubicSplineCash;
	std::array< std::array< std::array< Math::Vector3df, 200 >, 200>, 200 > spikyGrad;
};
	}
}