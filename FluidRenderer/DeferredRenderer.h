#ifndef __CRYSTAL_SHADER_DEFERRED_RENDERER_H__
#define __CRYSTAL_SHADER_DEFERRED_RENDERER_H__

#include "../../Crystal/Shader/ITextureObject.h"
#include "../../Crystal/Shader/ShaderObject.h"
#include "../../Crystal/Graphics/PointLight.h"
#include "../../Crystal/Graphics/Material.h"
#include "../../Crystal/Graphics/ICamera.h"

namespace Crystal {
	namespace Shader {

		class DeferredRenderer
		{
		public:
			void render(const ITextureObject& depthTexture, const ITextureObject& normalTexture, const Graphics::ICamera& renderedCamera, const Graphics::PointLight& light, const Graphics::Material& material);

			bool build();

		private:
			std::string getBuildinVertexShaderSource();

			std::string getBuildinFragmentShaderSource();

			void findLocation();

		private:
			ShaderObject shader;
		};

	}
}

#endif