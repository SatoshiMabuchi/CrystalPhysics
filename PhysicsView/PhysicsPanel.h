#pragma once

#include "../../Crystal/AppBase/IPanel.h"
#include "../Physics/STSPHSolver.h"
#include "PhysicsModel.h"
#include "PhysicsRenderer.h"

namespace Crystal {
	namespace UI {

class PhysicsPanel : public IPanel
{
public:
	PhysicsPanel(UI::PhysicsModel* model, UI::ICanvas* canvas, UI::PhysicsRenderer* renderer);

	void show() override;

private:
	bool isUnderSimulation;
	UI::PhysicsModel* model;
	UI::PhysicsRenderer* renderer;
};
	}
}