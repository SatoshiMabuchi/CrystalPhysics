#include "../../Crystal/AppBase/FileMenu.h"
#include "../../Crystal/AppBase/CameraMenu.h"
#include "../../Crystal/AppBase/Window.h"

#include "../../Crystal/UI/IModel.h"
#include "../../Crystal/UI/ICanvas.h"
#include "PhysicsPanel.h"
#include "PhysicsModel.h"

//#pragma comment(lib, "glew32.lib")

using namespace Crystal::Math;
using namespace Crystal::UI;
using namespace Crystal::Physics;

int main(int, char**)
{
	PhysicsModel model;
	ICanvas canvas;

	Window window(&model, &canvas);
	if (!window.init()) {
		assert(false);
	}

	window.add(new FileMenu(&model, &canvas));
	window.add(new CameraMenu(&model, &canvas));
	window.add(new PhysicsPanel(&model, &canvas));

	window.show();


	return 0;
}
