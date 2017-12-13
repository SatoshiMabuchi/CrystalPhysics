#pragma once

#include "../../Crystal/UI/IModel.h"
#include "../Physics/SPHParticle.h"
#include "../Physics/PBSPHSolver.h"

namespace Crystal {
	namespace Physics {
		class ISPHSolver;
		class ISPHParticle;
	}

	namespace UI {

class PhysicsModel : public UI::IModel
{
public:
	PhysicsModel();

	void clear();

	void addParticle(Physics::SPHParticle* object);

	UI::ViewModel toViewModel() const override;

	Math::Box3d getBoundingBox() const override;

	Physics::PBSPHSolver* getSolver() { return solver; }

private:
	Physics::PBSPHSolver* solver;
};
	}
}