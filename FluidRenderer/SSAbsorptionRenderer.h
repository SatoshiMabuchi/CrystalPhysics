#ifndef __CRYSTAL_SHADER_SS_ABSORPTION_RENDERER_H__
#define __CRYSTAL_SHADER_SS_ABSORPTION_RENDERER_H__

#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/Graphics/PointBuffer.h"
#include "../../Crystal/Shader/ITextureObject.h"
#include "../../Crystal/Shader/ShaderObject.h"

namespace Crystal {
	namespace Shader {

		class SSAbsorptionRenderer
		{
		public:
			void render(const ITextureObject& volumeTexture);

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
