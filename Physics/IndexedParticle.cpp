#include "IndexedParticle.h"

#include "SPHParticle.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;

void IndexedParticle::setGridID(const float effectLength)
{
	const auto& point = particle->getPosition();
	gridID = toGridId(point, effectLength);
}

int IndexedParticle::toIdX(std::array<int, 3> index)
{
	return (index[2] << 20) + (index[1] << 10) + index[0];
}

int IndexedParticle::toGridId(const Vector3df pos, const float effectLength)
{
	return toIdX(toIndex(pos, effectLength));
}

std::array<int,3> IndexedParticle::toIndex(const Vector3df pos, const float effectLength)
{
	const auto ix = static_cast<int>(pos.x / effectLength);
	const auto iy = static_cast<int>(pos.y / effectLength);
	const auto iz = static_cast<int>(pos.z / effectLength);
	return std::array<int, 3>{ix, iy, iz};
}