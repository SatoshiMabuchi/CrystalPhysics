#pragma once

#include "../../Crystal/UI/IModel.h"
#include "../Physics/PhysicsObject.h"

namespace Crystal {
	namespace Physics {
		class ISPHSolver;
	}

	namespace UI {

class PhysicsModel : public UI::IModel
{
public:
	PhysicsModel();

	void addPhysicsObject(Physics::PhysicsObject* object);

	//std::list<ParticleSystemObject> getParticleSystems() const { return objects.getParticleSystems(); }
	UI::ViewModel toViewModel() const override;

	Math::Box3d getBoundingBox() const override;

	Physics::ISPHSolver* getSolver() { return solver; }

private:
	std::list<Physics::PhysicsObject*> physicsObjects;
	Physics::ISPHSolver* solver;
};
	}
}