#include "PhysicsRenderer.h"

#include "../FluidRenderer/ParticleDepthRenderer.h"
#include "../FluidRenderer/BilateralFilter.h"

using namespace Crystal::Graphics;
using namespace Crystal::Shader;
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
	frameBuffer.build(512, 512);
	bluredDepthTexture.create(Imagef(512, 512, 1));
	if (!normalFilter.build()) {
		return false;
	}
	normalTexture.create(Imagef(512, 512, 2));
	if (!thicknessRenderer.build()) {
		return false;
	}
	thicknessTexture.create(Imagef(512, 512, 3));
	if (!deferredRenderer.build()) {
		return false;
	}
	shadedTexture.create(Imagef(512, 512), 4);
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

	frameBuffer.setTexture(bluredDepthTexture);
	glViewport(0, 0, bluredDepthTexture.getWidth(), bluredDepthTexture.getHeight());
	bluredDepthTexture.bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bilateralFilter.render(depthTexture);

	frameBuffer.setTexture(normalTexture);
	glViewport(0, 0, normalTexture.getWidth(), normalTexture.getHeight());
	normalTexture.bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	normalFilter.render(bluredDepthTexture, *camera);
	normalTexture.unbind();

	frameBuffer.setTexture(thicknessTexture);
	thicknessTexture.bind();
	thicknessRenderer.render(*camera, pointBuffer);
	thicknessTexture.unbind();

	PointLight light;
	light.setAmbient(glm::vec4(1, 1, 1, 1));
	Material material;
	material.setAmbient(glm::vec4(1, 1, 1, 1));
	material.setShininess(1.0);

	frameBuffer.setTexture(shadedTexture);
	shadedTexture.bind();
	deferredRenderer.render(bluredDepthTexture, normalTexture, *camera, light, material);
	shadedTexture.unbind();

	frameBuffer.unbind();

	glViewport(0, 0, width, height);
	onScreenRenderer.render(shadedTexture);
	glFlush();
}