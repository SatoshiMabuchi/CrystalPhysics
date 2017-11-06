#include "PhysicsPanel.h"

#include "../../Crystal/AppBase/imgui.h"

#include "../../Crystal/Math/Box3d.h"
#include "../Physics/PhysicsObject.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;
using namespace Crystal::UI;

PhysicsPanel::PhysicsPanel(IModel* model, ICanvas* canvas) :
	IPanel(model, canvas),
	isUnderSimulation(false)
{}

void PhysicsPanel::show()
{
	ImGui::Begin("Physics");
	if (ImGui::Button("Add")) {
		ImGui::OpenPopup("Add");
	}
	if (ImGui::BeginPopup("Add")) {
		static float point1[3] = { 0.0f, 0.0f, 0.0f };
		ImGui::InputFloat3("Point1", point1);
		static float point2[3] = { 1.0f, 1.0f, 1.0f };
		ImGui::InputFloat3("Point2", point2);
		static float divideLength = 0.1f;
		ImGui::InputFloat("DivideLength", &divideLength);
		static float density = 1000.0f;
		ImGui::InputFloat("Density", &density);
		static float pressureCoe = 10000.0f;
		ImGui::InputFloat("PressureCoe", &pressureCoe);
		static float viscosityCoe = 100.0f;
		ImGui::InputFloat("ViscosityCoe", &viscosityCoe);
		SPHConstant constant(density, pressureCoe, viscosityCoe, 0.0f, divideLength * 1.25);
		if (ImGui::Button("OK")) {
			Box3d box(Vector3df(0.0, 0.0, 0.0), Vector3df(1.0, 1.0, 1.0));
			//PhysicsObject* object = new PhysicsObject(box, divideLength, constant);
			//world.add(object);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Start")) {
		world.setExternalForce(Vector3df(0.0, -9.8, 0.0));
		world.setBoundary(Box3d(Vector3df(-100.0, 0.0, 0.0), Vector3df(100, 1.0, 1.0)));
		isUnderSimulation = !isUnderSimulation;
	}
	if (isUnderSimulation) {
		const float timeStep = 0.001f;
		world.simulate(0.125f, timeStep);
	}
	ImGui::End();
}

/*
std::vector<SPHParticle*> PhysicsPanel::getParticles()
{
	return world.getParticles();
}
*/
