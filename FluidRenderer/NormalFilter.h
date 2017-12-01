#ifndef __CRYSTAL_SHADER_NORMAL_FILTER_H__
#define __CRYSTAL_SHADER_NORMAL_FILTER_H__

#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/Graphics/PointBuffer.h"
#include "../../Crystal/Shader/ITextureObject.h"
#include "../../Crystal/Shader/ShaderObject.h"

namespace Crystal {
	namespace Shader {

class NormalFilter
{
public:
	void render(const ITextureObject& texture, const Graphics::ICamera& renderedCamera);

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