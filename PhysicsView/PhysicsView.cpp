#include "../../Crystal/AppBase/FileMenu.h"
#include "../../Crystal/AppBase/CameraMenu.h"
#include "../../Crystal/AppBase/Window.h"

#include "../../Crystal/UI/IModel.h"
#include "../../Crystal/UI/ICanvas.h"
#include "../../Crystal/Graphics/PerspectiveCamera.h"
#include "PhysicsPanel.h"
#include "PhysicsModel.h"
#include "PhysicsRenderer.h"

//#pragma comment(lib, "glew32.lib")

using namespace Crystal::Math;
using namespace Crystal::Graphics;
using namespace Crystal::UI;
using namespace Crystal::Physics;


int main(int, char**)
{
	auto camera = new PerspectiveCamera(
		1.0,
		0.5f * Tolerance<float>::getPI(),
		Vector3df(0, 0, 0),
		Vector3df(0, 0, -10.0),
		1.0f, 10.0f
	);

	auto renderer = new PhysicsRenderer(camera);

	PhysicsModel model;
	ICanvas canvas(renderer, camera);

	Window window(&model, &canvas);
	if (!window.init()) {
		assert(false);
	}

	window.add(new FileMenu(&model, &canvas));
	window.add(new CameraMenu(&model, &canvas));
	window.add(new PhysicsPanel(&model, &canvas, renderer));

	window.show();


	return 0;
}
