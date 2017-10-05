#include "IndexedParticle.h"

#include "SPHParticle.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void IndexedParticle::setGridID(const float effectLength)
{
	const Math::Vector3d<float>& point = particle->getPosition();
	gridID = toGridId(point, effectLength);
}

int IndexedParticle::toIdX(std::array<int, 3> index)
{
	return (index[2] << 20) + (index[1] << 10) + index[0];
}

int IndexedParticle::toGridId(const Vector3d<float> pos, const float effectLength)
{
	return toIdX(toIndex(pos, effectLength));
}

std::array<int,3> IndexedParticle::toIndex(const Vector3d<float> pos, const float effectLength)
{
	const auto ix = static_cast<int>(pos.getX() / effectLength);
	const auto iy = static_cast<int>(pos.getY() / effectLength);
	const auto iz = static_cast<int>(pos.getZ() / effectLength);
	return std::array<int, 3>{ix, iy, iz};
}