#pragma once

#include "../../Crystal/UI/IRenderer.h"
#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/UI/ViewModel.h"
#include "../../Crystal/Shader/PointRenderer.h"
#include "../../Crystal/Shader/DepthTextureObject.h"
#include "../../Crystal/Shader/OnScreenRenderer.h"
#include "../../Crystal/Shader/FrameBuffer.h"
#include "../../Crystal/Shader/DepthBuffer.h"
#include "../FluidRenderer/NormalFilter.h"
#include "../FluidRenderer/ParticleDepthRenderer.h"
#include "../FluidRenderer/BilateralFilter.h"
#include "../FluidRenderer/SSThicknessRenderer.h"

namespace Crystal {
	namespace UI {
class PhysicsRenderer : public IRenderer
{
public:
	explicit PhysicsRenderer(Graphics::ICamera* camera) :
		camera(camera)
	{}

	bool build();

	void setViewModel(const ViewModel& vm) override {
		this->pointBuffer = vm.getPointBuffer();
		this->pointRenderer.setBuffer( vm.getPointBuffer() );
	}

	void render(const int width, const int height) override;

private:
	Graphics::PointBuffer pointBuffer;
	Shader::PointRenderer pointRenderer;
	Shader::ParticleDepthRenderer depthRenderer;
	Shader::BilateralFilter bilateralFilter;
	Shader::DepthTextureObject depthTexture;
	Shader::OnScreenRenderer onScreenRenderer;
	Shader::SSThicknessRenderer thicknessRenderer;
	Shader::DepthBuffer depthBuffer;
	Shader::FrameBuffer frameBuffer;
	Shader::NormalFilter normalFilter;
	Shader::TextureObject bluredDepthTexture;
	Shader::TextureObject normalTexture;
	Shader::TextureObject thicknessTexture;

	Graphics::ICamera* camera;
};
	}
}
