#pragma once

#include "../../Crystal/Math/Vector3d.h"

namespace Crystal {
	namespace Physics {

class ISPHParticle
{
public:
	ISPHParticle(const Math::Vector3df& position, const float radius) :
		position(position),
		radius(radius),
		density(0.0)
	{}

	virtual ~ISPHParticle() {}

	Math::Vector3df getPosition() const { return position; }

	float getDiameter() const { return radius * 2.0f; }

	float getRadius() const { return radius; }

protected:
	Math::Vector3df position;
	float radius;
	float density;

};
	}
}