#include "../../Crystal/AppBase/FileMenu.h"
#include "../../Crystal/AppBase/Window.h"

#include "../../Crystal/UI/IModel.h"
#include "../../Crystal/UI/ICanvas.h"
#include "PhysicsPanel.h"

//#pragma comment(lib, "glew32.lib")

using namespace Crystal::Math;
using namespace Crystal::UI;

int main(int, char**)
{
	IModel model;
	ICanvas canvas;

	Window window(&model, &canvas);
	if (!window.init()) {
		assert(false);
	}

	window.add(new FileMenu(&model, &canvas));
	window.add(new PhysicsPanel(&model, &canvas));

	window.show();


	return 0;
}
