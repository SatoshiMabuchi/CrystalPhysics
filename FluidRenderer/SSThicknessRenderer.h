#ifndef __CRYSTAL_SHADER_SS_THICKNESS_RENDERER_H__
#define __CRYSTAL_SHADER_SS_THICKNESS_RENDERER_H__

#include "../../Crystal/Graphics/ICamera.h"
#include "../../Crystal/Graphics/PointBuffer.h"
#include "../../Crystal/Shader/ShaderObject.h"

namespace Crystal {
	namespace Shader {

class SSThicknessRenderer
{
public:
	void findLocation();

	void render(const Graphics::ICamera& camera, const Graphics::PointBuffer& buffer);

	bool build();

private:

	std::string getBuildinVertexShaderSource() const;

	std::string getBuildinFragmentShaderSource() const;

private:
	ShaderObject shader;
};

	}
}

#endif