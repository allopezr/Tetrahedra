#include "stdafx.h"
#include "ShaderProgramDB.h"

// Static attributes

std::unordered_map<uint8_t, std::string> Tet::ShaderProgramDB::RENDERING_SHADER_PATH{
		{RenderingShaderId::LINE_RENDERING, "Assets/Shaders/line"},
		{RenderingShaderId::POINT_RENDERING, "Assets/Shaders/point"},
		{RenderingShaderId::TRIANGLE_RENDERING, "Assets/Shaders/triangle"},
};

std::unordered_map<uint8_t, std::unique_ptr<Tet::RenderingShader>> Tet::ShaderProgramDB::_renderingShader;

// Private methods

Tet::ShaderProgramDB::ShaderProgramDB()
{
}

Tet::ShaderProgramDB::~ShaderProgramDB()
{
}

// Public methods

Tet::RenderingShader* Tet::ShaderProgramDB::getShader(RenderingShaderId shaderId)
{
	uint8_t shaderId8 = static_cast<uint8_t>(shaderId);

	if (!_renderingShader[shaderId8].get())
	{
		RenderingShader* shader = new RenderingShader();
		shader->createShaderProgram(RENDERING_SHADER_PATH.at(shaderId8).c_str());

		_renderingShader[shaderId8].reset(shader);
	}

	return _renderingShader[shaderId8].get();
}
