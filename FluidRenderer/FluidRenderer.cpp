#include "FluidRenderer.h"

#include "../../Crystal/Math/Vector2d.h"
#include "../../Crystal/Math/Vector3d.h"
#include "../../Crystal/Math/Box2d.h"
#include "../../Crystal/Graphics/Imagef.h"

#include <sstream>

using namespace Crystal::Math;
using namespace Crystal::Graphics;
using namespace Crystal::Shader;

bool FluidRenderer::build()
{
	const auto vsSource = getBuiltinVertexShaderSource();
	const auto fsSource = getBuiltinFragmentShaderSource();
	bool b = shader.build(vsSource, fsSource);
	findLocation();
	return b;
}

void FluidRenderer::findLocation()
{
	shader.findUniformLocation("surfaceTexture");
//	shader.findUniformLocation("reflectionTexture");
	//shader.findUniformLocation("refractionTexture");

	shader.findUniformLocation("absorptionTexture");
	shader.findUniformLocation("backgroundTexture");

	shader.findAttribLocation("position");
}

std::string FluidRenderer::getBuiltinVertexShaderSource()
{
	std::ostringstream stream;
	stream
		<< "#version 150" << std::endl
		<< "in vec2 position;" << std::endl
		<< "out vec2 texCoord;" << std::endl
		<< "void main(void) {" << std::endl
		<< "	texCoord = (position + vec2(1.0,1.0))/2.0;" << std::endl
		<< "	gl_Position = vec4(position, 0.0, 1.0);" << std::endl
		<< "}" << std::endl;
	ShaderUnit vertexShader;
	bool b = vertexShader.compile(stream.str(), ShaderUnit::Stage::VERTEX);
	return stream.str();
}

std::string FluidRenderer::getBuiltinFragmentShaderSource()
{
	std::ostringstream stream;
	stream
		<< "#version 150" << std::endl
		<< "uniform sampler2D surfaceTexture;" << std::endl
//		<< "uniform sampler2D reflectionTexture;" << std::endl
		//<< "uniform sampler2D refractionTexture;" << std::endl
		<< "uniform sampler2D absorptionTexture;" << std::endl
		<< "uniform sampler2D backgroundTexture;" << std::endl
		<< "in vec2 texCoord;" << std::endl
		<< "out vec4 fragColor;" << std::endl
		<< "void main(void) {" << std::endl
		<< "	vec4 surfaceColor = texture2D(surfaceTexture, texCoord);" << std::endl
//		<< "	vec4 reflectionColor = texture2D(reflectionTexture, texCoord);" << std::endl
		//<< "	vec4 refractionColor = texture2D(refractionTexture, texCoord);" << std::endl
		<< "	vec4 absorptionColor = texture2D(absorptionTexture, texCoord);" << std::endl
		<< "	vec4 bgColor = texture2D(backgroundTexture, texCoord);" << std::endl
		<< "	if(absorptionColor.a < 0.01) { " << std::endl
		<< "		fragColor = bgColor;" << std::endl
		<< "		return; " << std::endl
		<< "	}else {" << std::endl
		<< "		fragColor = mix( absorptionColor, surfaceColor, 1.0-absorptionColor.a);" << std::endl
		<< "	}" << std::endl
		<< "}" << std::endl;
	ShaderUnit fragmentShader;
	bool b = fragmentShader.compile(stream.str(), ShaderUnit::Stage::FRAGMENT);
	return stream.str();
}


void FluidRenderer::render(const Textures& textures)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const Box2d box(Vector2df(-1.0, -1.0), Vector2df(1.0, 1.0));
	const auto& positions = box.toArray();

	//glEnable(GL_DEPTH_TEST);

	glUseProgram(shader.getId());

	textures.surfaceTexture.bind();
	textures.reflectionTexture.bind();
	textures.absorptionTexture.bind();
	textures.backgroundTexture.bind();

	glUniform1i(shader.getUniformLocation("surfaceTexture"), textures.surfaceTexture.getId());
//	glUniform1i(shader.getUniformLocation("reflectionTexture"), textures.reflectionTexture.getId());

	glUniform1i(shader.getUniformLocation("absorptionTexture"), textures.absorptionTexture.getId());
	glUniform1i(shader.getUniformLocation("backgroundTexture"), textures.backgroundTexture.getId());

	glVertexAttribPointer(shader.getAttribLocation("position"), 2, GL_FLOAT, GL_FALSE, 0, positions.data());

	glEnableVertexAttribArray(0);
	glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(positions.size()) / 2);
	glDisableVertexAttribArray(0);

	glBindFragDataLocation(shader.getId(), 0, "fragColor");

	textures.surfaceTexture.unbind();
	textures.reflectionTexture.unbind();
	textures.absorptionTexture.unbind();
	textures.backgroundTexture.unbind();

	glUseProgram(0);
}