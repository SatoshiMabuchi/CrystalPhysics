#include "PhysicsPanel.h"

#include "../../Crystal/AppBase/imgui.h"

#include "../../Crystal/Math/Box3d.h"
#include "../../Crystal/UI/ICanvas.h"
#include "../Physics/PhysicsObject.h"

using namespace Crystal::Math;
using namespace Crystal::Physics;
using namespace Crystal::UI;

PhysicsPanel::PhysicsPanel(PhysicsModel* model, ICanvas* canvas) :
	IPanel(model, canvas),
	model(model),
	isUnderSimulation(false)
{}

void PhysicsPanel::show()
{
	ImGui::Begin("Physics");
	if (ImGui::Button("Add")) {
		ImGui::OpenPopup("Add");
	}
	if (ImGui::BeginPopup("Add")) {
		static float point1[3] = { 0.0f, 1.0f, 0.0f };
		ImGui::InputFloat3("Point1", point1);
		static float point2[3] = { 20.0f, 10.0f, 20.0f };
		ImGui::InputFloat3("Point2", point2);
		static float divideLength = 1.0f;
		ImGui::InputFloat("DivideLength", &divideLength);

		static float density = 1000.0f;
		ImGui::InputFloat("Density", &density);
		static float pressureCoe = 10000.0f;
		ImGui::InputFloat("PressureCoe", &pressureCoe);
		static float viscosityCoe = 500.0f;
		ImGui::InputFloat("ViscosityCoe", &viscosityCoe);
		SPHConstant constant(density, pressureCoe, viscosityCoe, 0.0f, divideLength * 1.25);
		if (ImGui::Button("OK")) {
			PhysicsObject* object = new PhysicsObject(constant);
			for (auto x = point1[0]; x < point2[0]; x += divideLength) {
				for (auto y = point1[1]; y < point2[1]; y += divideLength) {
					for (auto z = point1[2]; z < point2[2]; z += divideLength) {
						object->createParticle(Vector3df(x, y, z), Vector3df(0, 0, 0));
					}
				}
			}
			model->addPhysicsObject(object);
			canvas->setViewModel(model->toViewModel());
			//canvas->fitCamera(model->getBoundingBox());
			//object->
			//box, divideLength, constant);
			//world.add(object);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Start")) {
		isUnderSimulation = !isUnderSimulation;
	}
	if (isUnderSimulation) {
		const float timeStep = 0.03f;
		model->getSolver()->simulate(1.25f, timeStep);
		canvas->setViewModel(model->toViewModel());
	}
	ImGui::End();
}

/*
std::vector<SPHParticle*> PhysicsPanel::getParticles()
{
	return world.getParticles();
}
*/
