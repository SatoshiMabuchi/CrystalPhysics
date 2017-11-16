#pragma once

#include "../../Crystal/UI/IRenderer.h"
#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/UI/ViewModel.h"
#include "../../Crystal/Shader/PointRenderer.h"

namespace Crystal {
	namespace UI {
		class PhysicsRenderer : public IRenderer
		{
		public:
			explicit PhysicsRenderer(Graphics::ICamera* camera) :
				camera(camera)
			{}

			bool build()
			{
				return pointRenderer.build();
			}

			void setViewModel(const ViewModel& vm) override {
				this->pointBuffer = vm.getPointBuffer();
			}

			void render(const int width, const int height) override
			{
				glClearColor(0.0, 0.0, 1.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				pointRenderer.render(*camera, pointBuffer);
			}

		private:
			Shader::PointRenderer pointRenderer;
			Graphics::PointBuffer pointBuffer;
			Graphics::ICamera* camera;
		};
	}
}
