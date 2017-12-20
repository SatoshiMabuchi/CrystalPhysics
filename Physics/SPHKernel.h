#ifndef __CRYSTAL_PHYSICS_SPH_KERNEL_H__
#define __CRYSTAL_PHYSICS_SPH_KERNEL_H__

#include "../../Crystal/Math/Vector3d.h"
#include "../../Crystal/Math/Matrix3d.h"

namespace Crystal {
	namespace Physics {

class SPHKernel
{
public:
	SPHKernel() = default;

	SPHKernel(const float effectLength);

	float getPoly6Kernel(const float distance);

	float getPoly6Kernel2(const float distanceSquared);

	float getPoly6Kernel(const float distance, const float effectLength);

	Math::Vector3df getPoly6KernelGradient(const Math::Vector3df& distanceVector, const float effectLength);

	float getPoly6KernelGradientCoe(const float distance, const float effectLength);

	float getPoly6KernelLaplacian(const float distance, const float effectLength);

	Math::Vector3df getSpikyKernelGradient(const Math::Vector3df& distanceVector);

	float getSpikyKernelGradientWeight(const float distance, const float effectLength);

	Math::Vector3df getSpikyKernelGradient(const Math::Vector3df& distanceVector, const float effectLength);

	float getViscosityKernelLaplacian(const float distance, const float effectLength);

	float getCubicSpline(const float q);

	float getCubicSpline(const float distance, const float effectRadius);

	Math::Vector3df getCubicSplineGradient(const Math::Vector3df& distanceVector);

	Math::Vector3df getCubicSplineGradient(const Math::Vector3df& distanceVector, const float effectRadius);

	float getCubicSpline(const Math::Vector3df& v, const float effectLength);

	float getEffectLength() const { return effectLength; }

private:
	float effectLength;
	float poly6KernelConstant;
	float spikyKernelGradConstant;
	float effectLengthSquared;
};
	}
}

#endif