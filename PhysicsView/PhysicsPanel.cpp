#include "PhysicsPanel.h"

#include "../../Crystal/AppBase/imgui.h"

#include "../../Crystal/Math/Box3d.h"
#include "../../Crystal/UI/ICanvas.h"
#include "../Physics/SPHConstant.h"
#include "../Physics/ISPHParticle.h"
#include "../Physics/SPHParticle.h"
#include "../Physics/PBSPHParticle.h"
#include "../Physics/SPHKernelCache.h"
#include <iostream>

using namespace Crystal::Math;
using namespace Crystal::Physics;
using namespace Crystal::UI;

PhysicsPanel::PhysicsPanel(PhysicsModel* model, ICanvas* canvas, PhysicsRenderer* renderer) :
	IPanel(model, canvas),
	model(model),
	renderer(renderer),
	isUnderSimulation(false)
{}

void PhysicsPanel::show()
{
	ImGui::Begin("Physics");
	if (ImGui::Button("Clear")) {
		ImGui::OpenPopup("Clear");
		model->clear();
		canvas->setViewModel(model->toViewModel());
	}

	if (ImGui::Button("Boundary")) {
		ImGui::OpenPopup("Boundary");
	}
	if (ImGui::BeginPopup("Boundary")) {
		static float point1[3] = { -100.0f, 0.0f, -10.0f };
		ImGui::InputFloat3("Point1", point1);
		static float point2[3] = { 100.0f, 100.0f, 10.0f };
		ImGui::InputFloat3("Point2", point2);

		if (ImGui::Button("OK")) {
			Box3d box(Vector3df( point1[0], point1[1], point1[2] ),
				Vector3df(point2[0], point2[1], point2[2] ) );
			model->getSolver()->setBoundary(box);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("ExternalForce")) {
		ImGui::OpenPopup("ExternalForce");
	}
	if (ImGui::BeginPopup("ExternalForce")) {
		static glm::vec3 point1 = { 0.0f, -9.8f, 0.0f };
		ImGui::InputFloat3("Force", &point1[0]);

		if (ImGui::Button("OK")) {
			model->getSolver()->setExternalForce(point1);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}



	float effectLength = 1.15f;

	if (ImGui::Button("Add")) {
		ImGui::OpenPopup("Add");
	}
	if (ImGui::BeginPopup("Add")) {
		static float point1[3] = { 0.0f, -0.5f, -10.0f };
		ImGui::InputFloat3("Point1", point1);
		static float point2[3] = { 20.0f, 10.0f, 10.0f };
		ImGui::InputFloat3("Point2", point2);
		static float divideLength = 1.0f;
		ImGui::InputFloat("DivideLength", &divideLength);

		static float density = 1.0f;
		ImGui::InputFloat("Density", &density);
		static float pressureCoe = 10000.0f;
		ImGui::InputFloat("PressureCoe", &pressureCoe);
		static float viscosityCoe = 500.0f;
		ImGui::InputFloat("ViscosityCoe", &viscosityCoe);

		SPHConstant* constant = new SPHConstant(density, pressureCoe, viscosityCoe, 0.0f, effectLength);
		if (ImGui::Button("OK")) {
			for (auto x = point1[0]; x < point2[0]; x += divideLength) {
				for (auto y = point1[1]; y < point2[1]; y += divideLength) {
					for (auto z = point1[2]; z < point2[2]; z += divideLength) {
						const Vector3df position(x, y, z);
						auto p = new PBSPHParticle(position, divideLength * 0.5, constant);
						model->getSolver()->add(p);
					}
				}
			}
			SPHKernelCache::getInstance()->build(effectLength);

			canvas->setViewModel(model->toViewModel());
			//std::cout << object->getParticles().size() << std::endl;
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
		const float timeStep = 0.20f;
		for (int i = 0; i < 1; ++i) {
			model->getSolver()->simulate(timeStep, effectLength, effectLength * 1.0f, 3);
		}
		std::cout << model->getSolver()->getParticles().size() << ", " //<< std::endl;
			<< model->getSolver()->getParticles().front()->getDensity() << std::endl;
		canvas->setViewModel(model->toViewModel());
	}
	if (ImGui::Button("Sprite")) {
		static bool showSprite;
		showSprite = !showSprite;
		renderer->setShowSprite(showSprite);
	}

	ImGui::End();
}

/*
std::vector<SPHParticle*> PhysicsPanel::getParticles()
{
	return world.getParticles();
}
*/
