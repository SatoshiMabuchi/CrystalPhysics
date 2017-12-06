#include "PhysicsRenderer.h"

#include "../FluidRenderer/ParticleDepthRenderer.h"
#include "../FluidRenderer/BilateralFilter.h"

#include "../../Crystal/IO/ImageFileReader.h"

using namespace Crystal::Graphics;
using namespace Crystal::Shader;
using namespace Crystal::IO;
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

	/*
	ImageFileReader reader;
	if (!reader.read("../ThirdParty/forest-skyboxes/Brudslojan/posx.jpg")) {
		return false;
	}
	std::array<Imagef, 6> images;
	images[0] = reader.getImage();
	if (!reader.read("../ThirdParty/forest-skyboxes/Brudslojan/negx.jpg")) {
		return false;
	}
	images[1] = reader.getImage();
	if (!reader.read("../ThirdParty/forest-skyboxes/Brudslojan/posy.jpg")) {
		return false;
	}
	images[2] = reader.getImage();
	if (!reader.read("../ThirdParty/forest-skyboxes/Brudslojan/negy.jpg")) {
		return false;
	}
	images[3] = reader.getImage();

	if (!reader.read("../ThirdParty/forest-skyboxes/Brudslojan/posz.jpg")) {
		return false;
	}
	images[4] = reader.getImage();
	if (!reader.read("../ThirdParty/forest-skyboxes/Brudslojan/negz.jpg")) {
		return false;
	}
	images[5] = reader.getImage();

	cubeMapTexture.create(images, 5);
	*/

	reflectionTexture.create(Imagef(512, 512), 6);

	if (!reflectionRenderer.build()) {
		return false;
	}

	if (!absorptionRenderer.build()) {
		return false;
	}
	volumeTexture.create(Imagef(512, 512), 7);

	backgroundTexture.create(Imagef(512, 512), 8);

	if (!fluidRenderer.build()) {
		return false;
	}
	fluidTexture.create(Imagef(512, 512), 9);


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
	light.setPos(glm::vec3(0, 100, 0));
	light.setAmbient(glm::vec4(1, 1, 1, 1));
	light.setDiffuse(glm::vec4(1, 1, 1, 1));
	light.setSpecular(glm::vec4(1, 1, 1, 1));
	Material material;
	material.setAmbient(glm::vec4(0, 0, 1, 0));
	material.setDiffuse(glm::vec4(1, 0, 0, 0));
	material.setSpecular(glm::vec4(0, 1, 0, 0));
	material.setShininess(1.0);

	frameBuffer.setTexture(shadedTexture);
	shadedTexture.bind();
	deferredRenderer.render(bluredDepthTexture, normalTexture, *camera, light, material);
	shadedTexture.unbind();

	frameBuffer.setTexture(reflectionTexture);
	reflectionTexture.bind();
	reflectionRenderer.render(bluredDepthTexture, normalTexture, *camera, cubeMapTexture);
	reflectionTexture.unbind();

	frameBuffer.setTexture(thicknessTexture);
	thicknessTexture.bind();
	thicknessRenderer.render(*camera, pointBuffer);
	thicknessTexture.unbind();

	frameBuffer.setTexture(volumeTexture);
	volumeTexture.bind();
	absorptionRenderer.render(thicknessTexture);
	volumeTexture.unbind();

	FluidRenderer::Textures textures;
	textures.absorptionTexture = this->volumeTexture;
	textures.backgroundTexture = this->backgroundTexture;
	textures.reflectionTexture = this->reflectionTexture;
	textures.surfaceTexture = this->shadedTexture;

	frameBuffer.setTexture(fluidTexture);
	fluidTexture.bind();
	fluidRenderer.render(textures);
	fluidTexture.unbind();

	frameBuffer.unbind();

	glViewport(0, 0, width, height);
	onScreenRenderer.render(fluidTexture);
	glFlush();
}