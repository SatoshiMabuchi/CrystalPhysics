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

	float getPoly6(const float v) const;

	float getPoly6GradCoe(const float v) const;

private:
	std::array<float, 200> poly6;
	std::array<float, 200> poly6GradCoe;
	std::array<float, 200> cubicSplineCash;
	std::array<float, 200 > spikyGrad;
};
	}
}