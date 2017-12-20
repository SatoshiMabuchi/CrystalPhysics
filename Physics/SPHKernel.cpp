#include "SPHKernel.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

SPHKernel::SPHKernel(const float effectLength) :
	effectLength(effectLength)
{
	this->poly6KernelConstant = 315.0f / (64.0f * Math::Tolerance<float>::getPI() * pow(effectLength, 9));
	this->spikyKernelGradConstant = 45.0f / (Math::Tolerance<float>::getPI() * pow(effectLength, 6));
	this->effectLengthSquared = effectLength * effectLength;
}

float SPHKernel::getPoly6Kernel(const float distance)
{
	if (distance > effectLength) {
		return 0.0f;
	}
	return this->poly6KernelConstant * pow(effectLength * effectLength - distance * distance, 3);
}

float SPHKernel::getPoly6Kernel2(const float distanceSquared)
{
	if (distanceSquared > effectLengthSquared) {
		return 0.0f;
	}
	const auto a = effectLengthSquared - distanceSquared;
	return this->poly6KernelConstant * a * a * a;
}

float SPHKernel::getPoly6Kernel(const float distance, const float effectLength)
{
	if (distance > effectLength) {
		return 0.0f;
	}
	const auto poly6Constant = 315.0f / (64.0f * Math::Tolerance<float>::getPI() * pow(effectLength, 9));
	return poly6Constant * pow(effectLength * effectLength - distance * distance, 3);
}

Vector3df SPHKernel::getPoly6KernelGradient(const Vector3df& distanceVector, const float effectLength)
{
	const auto distance = glm::length(distanceVector);
	if (distance > effectLength) {
		return Vector3df(0, 0, 0);
	}
	const auto poly6ConstantGradient = 945.0f / (32.0f * Tolerance<float>::getPI() * pow(effectLength, 9));
	const auto factor = poly6ConstantGradient * pow(effectLength * effectLength - distance * distance, 2);
	return distanceVector * factor;
}

float SPHKernel::getPoly6KernelGradientCoe(const float distance, const float effectLength)
{
	const auto poly6ConstantGradient = 945.0f / (32.0f * Tolerance<float>::getPI() * pow(effectLength, 9));
	return poly6ConstantGradient * pow(effectLength * effectLength - distance * distance, 2);
}


float SPHKernel::getPoly6KernelLaplacian(const float distance, const float effectLength)
{
	if (distance > effectLength) {
		return 0.0f;
	}
	const auto poly6ConstantLaplacian = 945.0f / (32.0f * Tolerance<float>::getPI() * pow(effectLength, 9));
	return poly6ConstantLaplacian * (effectLength * effectLength - distance * distance)
		* (42.0f * distance * distance - 18.0f * effectLength * effectLength);
}

Vector3df SPHKernel::getSpikyKernelGradient(const Vector3df& distanceVector)
{
	const auto distance = glm::length( distanceVector );
	if (distance > effectLength) {
		return Vector3df(0, 0, 0);
	}
	return distanceVector * this->spikyKernelGradConstant * pow(effectLength - distance, 2) / distance;
}

float SPHKernel::getSpikyKernelGradientWeight(const float distance, const float effectLength)
{
	if (distance > effectLength) {
		return 0.0;
	}
	const auto constant = 45.0f / (Math::Tolerance<float>::getPI() * pow(effectLength, 6));
	return constant * pow(effectLength - distance, 2) / distance;
}

Vector3df SPHKernel::getSpikyKernelGradient(const Vector3df& distanceVector, const float effectLength)
{
	const auto distance = glm::length( distanceVector );
	if (distance > effectLength) {
		return Vector3df(0, 0, 0);
	}
	const auto constant = 45.0f / (Math::Tolerance<float>::getPI() * pow(effectLength, 6));
	return distanceVector * constant * pow(effectLength - distance, 2) / distance;
}

float SPHKernel::getViscosityKernelLaplacian(const float distance, const float effectLength)
{
	if (distance > effectLength) {
		return 0.0f;
	}
	const auto constant = 45.0f / (Math::Tolerance<float>::getPI() * pow(effectLength, 6));
	return (effectLength - distance) * constant;
}

float SPHKernel::getCubicSpline(const float q)
{
	const auto coe = 3.0f / Tolerance<float>::getTwoPI();
	if (q < 1) {
		return coe * (2.0f / 3.0f-q*q + 0.5f*q*q*q);
	}
	else if (q < 2) {
		return coe * (std::pow(2 - q, 3) / 6.0f);
	}
	else {
		return 0;
	}
}

float SPHKernel::getCubicSpline(const float distance, const float effectRadius)
{
	const auto q = distance * 2 / (effectRadius);
	return getCubicSpline(q);
}

Vector3df SPHKernel::getCubicSplineGradient(const Vector3df& distanceVector)
{
	const auto coe = 3.0f / Tolerance<float>::getTwoPI();
	const auto length = glm::length( distanceVector );
	const auto lengthSquared = getLengthSquared( distanceVector );
	if (Tolerance<float>::isZeroStrictly(length)) {
		return Vector3df(0, 0, 0);
	}
	else if (length < 1) {
		const auto var = -2.0f *length + 1.5f *lengthSquared;
		return coe * var * glm::normalize( distanceVector );
	}
	else if (length < 2) {
		const auto var = -0.5f * std::pow(2.0f - length, 2.0f);
		return coe * var * glm::normalize( distanceVector );
	}
	else {
		return Vector3df(0, 0, 0);
	}
}

Vector3df SPHKernel::getCubicSplineGradient(const Vector3df& distanceVector, const float effectRadius)
{
	const auto scale = glm::length( distanceVector ) * 2.0f / effectRadius;
	return getCubicSplineGradient(distanceVector * scale);
}

float SPHKernel::getCubicSpline(const Vector3df& v, const float effectLength)
{
	const auto q = glm::length(v) / effectLength;
	const auto numerator = getCubicSpline(q);
	const auto denominator = std::pow(effectLength, 3);
	return numerator / denominator;
}
