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

	float getSpikyGradient(const float v) const;

private:
	std::array<float, 200> cubicSplineCash;
	std::array<float, 200 > spikyGrad;
};
	}
}