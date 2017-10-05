#include <stdio.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

#include "../../Crystal/AppBase/FileMenu.h"
#include "../../Crystal/AppBase/Window.h"

#include "../../Crystal/UI/IModel.h"
#include "../../Crystal/UI/ICanvas.h"
#include "PhysicsPanel.h"

//#pragma comment(lib, "glew32.lib")

using namespace Crystal::Math;
using namespace Crystal::UI;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
	// Setup window
	glfwSetErrorCallback(error_callback);

	IModel model;
	ICanvas canvas;

	Window window;
	if (!window.init()) {
		assert(false);
	}

	window.add(new FileMenu(&model, &canvas));
	window.add(new PhysicsPanel(&model, &canvas));

	//PointRenderer renderer;
	//renderer.build();

	window.show();


	return 0;
}
