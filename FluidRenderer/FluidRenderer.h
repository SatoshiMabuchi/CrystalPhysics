#ifndef __CRYSTAL_SHADER_FLUID_RENDERER_H__
#define __CRYSTAL_SHADER_FLUID_RENDERER_H__

#include "ParticleDepthRenderer.h"
#include "../../Crystal/Shader/DepthTextureObject.h"
#include "../../Crystal/Shader/ITextureObject.h"
#include "../../Crystal/Shader/TextureObject.h"
#include "../../Crystal/Shader/FrameBuffer.h"

namespace Crystal {
	namespace Shader {

class FluidRenderer
{
public:
	struct Textures
	{
		TextureObject surfaceTexture;
		TextureObject reflectionTexture;
		TextureObject absorptionTexture;
		TextureObject backgroundTexture;
	};

	bool build();

	void render(const Textures& textures);

private:
	std::string getBuiltinVertexShaderSource();

	std::string getBuiltinFragmentShaderSource();

	void findLocation();

	ShaderObject shader;
};

	}
}

#endif