#include "PhysicsRenderer.h"

#include "../FluidRenderer/ParticleDepthRenderer.h"
#include "../FluidRenderer/BilateralFilter.h"

using namespace Crystal::Graphics;
using namespace Crystal::UI;

bool PhysicsRenderer::build()
{
	if (!pointRenderer.build()) {
		return false;
	}
	if (!depthRenderer.build()) {
		return false;
	}
	if (!bilateralFilter.build()) {
		return false;
	}
	if (!onScreenRenderer.build()) {
		return false;
	}
	depthTexture.create(Imagef(512, 512), 0);
	if (!depthBuffer.build(depthTexture)) {
		return false;
	}
	return true;
}


void PhysicsRenderer::render(const int width, const int height)
{
	depthBuffer.setTexture(depthTexture);
	glViewport(0, 0, depthBuffer.getWidth(), depthBuffer.getHeight());
	depthBuffer.bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	depthRenderer.render(*camera, pointBuffer);
	depthBuffer.unbind();

	glViewport(0, 0, width, height);
	onScreenRenderer.render(depthTexture);
	glFlush();
	/*
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//pointRenderer.render(*camera);

	depthTexture.bind();
	glViewport(0, 0, 512, 512);
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	depthRenderer.render(*camera, pointBuffer);
	glFlush();
	depthTexture.unbind();

	glViewport(0, 0, width, height);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	onScreenRenderer.render(depthTexture);
	glFlush();
	//bilateralFilter.render()
	*/
}