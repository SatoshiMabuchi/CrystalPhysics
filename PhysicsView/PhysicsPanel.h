#pragma once

#include "../../Crystal/AppBase/IPanel.h"
#include "../Physics/STSPHSolver.h"
#include "PhysicsModel.h"

namespace Crystal {
	namespace UI {

class PhysicsPanel : public IPanel
{
public:
	PhysicsPanel(UI::PhysicsModel* model, UI::ICanvas* canvas);

	void show() override;

private:
	bool isUnderSimulation;
	UI::PhysicsModel* model;
};
	}
}