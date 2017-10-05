#pragma once

#include "../../Crystal/AppBase/IPanel.h"
#include "../Physics/PhysicsWorld.h"

namespace Crystal {
	namespace UI {

class PhysicsPanel : public IPanel
{
public:
	PhysicsPanel(UI::IModel* model, UI::ICanvas* canvas);

	void show() override;

private:
	bool isUnderSimulation;
	Physics::PhysicsWorld world;
};
	}
}