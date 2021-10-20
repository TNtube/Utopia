#pragma once

#include <initializer_list>

#include "Cardia/Renderer/Shader.hpp"

namespace Cardia
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::initializer_list<std::string> filePaths);
		void bind() const override;
		void unbind() const override;

		virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;
		virtual void setInt(const std::string& name, int value) override;

		void setUniformMat4(const std::string& name, glm::mat4 matrix) const;
		void setUniformFloat4(const std::string& name, glm::vec4 data) const;
		void setUniformFloat3(const std::string& name, glm::vec3 data) const;
		void setUniformInt(const std::string& name, int value) const;
	private:
		uint32_t m_ShaderID;
	};
}