#include "SPHKernelCache.h"
#include "SPHKernel.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void SPHKernelCache::build(const float effectLength)
{
	SPHKernel kernel(effectLength);
	for (int i = 0; i < 200; ++i) {
		cubicSplineCash[i] = kernel.getCubicSpline(i / 200.0f * effectLength, effectLength);
	}
	for (int i = 0; i < 200; ++i) {
		for (int j = 0; j < 200; ++j) {
			for (int k = 0; k < 200; ++k) {
				Vector3df vec(i - 100, j - 100, k - 100);
				vec /= 100.0f;
				vec *= effectLength;
				spikyGrad[i][j][k] = kernel.getSpikyKernelGradient(vec, effectLength);
			}
		}
	}
}

float SPHKernelCache::getCubicSpline(const float v) const
{
	const auto i = v * 200;
	if (i >= 200) {
		return 0.0;
	}
	return cubicSplineCash[i];
}

Vector3df SPHKernelCache::getSpikyGradient(const Vector3df& v) const
{
	auto ii = v * 100.0f;
	ii += glm::ivec3(100, 100, 100);
	if (ii.x < 0 || ii.x <= 200) {
		return Vector3df(0, 0, 0);
	}
	if (ii.y < 0 || ii.y <= 200) {
		return Vector3df(0, 0, 0);
	}
	if (ii.z < 0 || ii.z <= 200) {
		return Vector3df(0, 0, 0);
	}
	return spikyGrad[ii.x][ii.y][ii.z];
}
