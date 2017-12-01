#ifndef __CRYSTAL_SHADER_PARTICLE_DEPTH_RENDERER_H__
#define __CRYSTAL_SHADER_PARTICLE_DEPTH_RENDERER_H__

#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/Shader/ShaderObject.h"
#include "../../Crystal/Graphics/PointBuffer.h"

namespace Crystal {
	namespace Shader {

class ParticleDepthRenderer
{
public:
	void render(const Graphics::ICamera& camera, const Graphics::PointBuffer& buffer);

	bool build();

private:
	std::string getBuildinVertexShaderSource();

	std::string getBuildinFragmentShaderSource();

	void findLocation();


private:
	ShaderObject shader;
	ShaderUnit vertexShader;
	ShaderUnit fragmentShader;
};

	}
}

#endif
