#include "SSReflectionRenderer.h"

#include "../../Crystal/Math/Vector2d.h"
#include "../../Crystal/Math/Box2d.h"
#include <sstream>

using namespace Crystal::Math;
using namespace Crystal::Graphics;
using namespace Crystal::Shader;

bool SSReflectionRenderer::build()
{
	const auto vsSource = getBuildinVertexShaderSource();
	const auto fsSource = getBuildinFragmentShaderSource();
	bool b = shader.build(vsSource, fsSource);
	findLocation();
	return b;
}

std::string SSReflectionRenderer::getBuildinVertexShaderSource()
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

std::string SSReflectionRenderer::getBuildinFragmentShaderSource()
{
	std::ostringstream stream;
	stream
		<< "#version 150" << std::endl
		<< "in vec2 texCoord;" << std::endl
		<< "out vec4 fragColor;" << std::endl
		<< "uniform mat4 projectionMatrix;" << std::endl
		<< "uniform vec3 eyePosition;" << std::endl
		<< "uniform sampler2D depthTex;" << std::endl
		<< "uniform sampler2D normalTex;" << std::endl
		<< "uniform samplerCube cubeMapTex;" << std::endl
		<< "float getDepth(vec2 tCoord){" << std::endl
		<< "	return texture2D(depthTex, tCoord).r;" << std::endl
		<< "}" << std::endl
		<< "vec3 uvToEye(vec2 tCoord, float depth) {" << std::endl
		<< "	vec4 clippingPosition = vec4(tCoord, depth, 1.0);" << std::endl
		<< "	vec4 viewPosition = inverse(projectionMatrix) * clippingPosition;" << std::endl
		<< "    return viewPosition.xyz / viewPosition.w;" << std::endl
		<< "}" << std::endl
		<< "vec3 getEyePosition(vec2 texCoord){"
		<< "	float depth = getDepth(texCoord);" << std::endl
		<< "	return uvToEye(texCoord, depth);" << std::endl
		<< "}" << std::endl
		<< "void main(void) {" << std::endl
		<< "	float depth = getDepth(texCoord);" << std::endl
		<< "	if(depth < 0.01) {" << std::endl
		<< "		fragColor = vec4(0.0, 0.0, 0.0, 0.0);" << std::endl
		<< "		return;" << std::endl
		<< "	}" << std::endl
		<< "	vec3 normal = texture2D(normalTex, texCoord).rgb;" << std::endl
		<< "    vec3 position = getEyePosition(texCoord);" << std::endl
		<< "	vec3 worldView = normalize( eyePosition - position );" << std::endl
		<< "	float innerProduct = dot(-worldView, normal);" << std::endl
		<< "	float fresnelBias = pow( (1.0-1.33) /(1.0+1.33), 2);" << std::endl
		<< "	float fresnel = fresnelBias + ( 1.0 - fresnelBias ) * pow(1.0 - innerProduct, 5); " << std::endl
		<< "	vec3 reflectDir = reflect(-worldView, normal);" << std::endl
		<< "	vec3 refractDir = refract(-worldView, normal, 1.33);" << std::endl
		<< "	vec4 reflectColor = texture(cubeMapTex, reflectDir);" << std::endl
		<< "	vec4 refractColor = texture(cubeMapTex, refractDir);" << std::endl
		<< "	fragColor = mix( reflectColor,refractColor,fresnel);" << std::endl
		<< "	fragColor = reflectColor;" << std::endl
		//<< "	fragColor = vec4(fresnel);" << std::endl

		<< "	fragColor.a = 1.0f;" << std::endl
		<< "}" << std::endl;
	ShaderUnit fragmentShader;
	bool b = fragmentShader.compile(stream.str(), ShaderUnit::Stage::FRAGMENT);
	return stream.str();
}

void SSReflectionRenderer::findLocation()
{
	shader.findUniformLocation("projectionMatrix");

	shader.findUniformLocation("depthTex");
	shader.findUniformLocation("normalTex");
	shader.findUniformLocation("cubeMapTex");

	shader.findUniformLocation("eyePosition");
	shader.findAttribLocation("position");
}

void SSReflectionRenderer::render(const ITextureObject& depthTexture, const ITextureObject& normalTexture, const ICamera& renderedCamera, const CubeMapTextureObject& cubeMapTexture)
{
	const Box2d box(Vector2df(-1.0f, -1.0f), Vector2df(1.0f, 1.0f));
	const auto& positions = box.toArray();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);

	glUseProgram(shader.getId());

	depthTexture.bind();
	normalTexture.bind();
	cubeMapTexture.bind();

	const auto& matrix = renderedCamera.getProjectionMatrix();
	const auto& pos = renderedCamera.getPosition();
	glUniformMatrix4fv(shader.getUniformLocation("projectionMatrix"), 1, GL_FALSE, &matrix[0][0]);
	glUniform3fv(shader.getUniformLocation("eyePosition"), 1, &pos[0]);

	glUniform1i(shader.getUniformLocation("depthTex"), depthTexture.getId());
	glUniform1i(shader.getUniformLocation("normalTex"), normalTexture.getId());
	glUniform1i(shader.getUniformLocation("cubeMapTex"), cubeMapTexture.getId());


	glVertexAttribPointer(shader.getAttribLocation("position"), 2, GL_FLOAT, GL_FALSE, 0, positions.data());

	glEnableVertexAttribArray(0);
	glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(positions.size() / 2));
	glDisableVertexAttribArray(0);

	glBindFragDataLocation(shader.getId(), 0, "fragColor");

	cubeMapTexture.unbind();
	depthTexture.unbind();
	normalTexture.unbind();
	glDisable(GL_DEPTH_TEST);

	glUseProgram(0);
}