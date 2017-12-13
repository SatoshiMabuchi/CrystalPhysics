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
		spikyGrad[i] = kernel.getSpikyKernelGradientWeight(i / 200.0f * effectLength, effectLength);
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

float SPHKernelCache::getSpikyGradient(const float v) const
{
	const auto i = v * 200;
	if (i >= 200) {
		return 0.0;
	}
	return spikyGrad[i];
}
