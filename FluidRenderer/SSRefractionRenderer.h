#ifndef __CRYSTAL_SS_REFRACTION_RENDERER_H__
#define __CRYSTAL_SS_REFRACTION_RENDERER_H__

#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/Graphics/PointBuffer.h"
#include "../../Crystal/Shader/ShaderObject.h"
#include "../../Crystal/Shader/ITextureObject.h"
#include "../../Crystal/Shader/CubeMapTextureObject.h"

namespace Crystal {
	namespace Shader {

		class SSRefractionRenderer
		{
		public:
			void render(const ITextureObject& depthTexture, const ITextureObject& normalTexture, const Graphics::ICamera& renderedCamera, const CubeMapTextureObject& cubeMapTexture);

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