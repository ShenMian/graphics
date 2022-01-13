// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include "Shader.h"
#include <vulkan/vulkan.h>
#include <unordered_map>
#include <vector>

class Shader;
class VKShader;

class VKProgram : public Program
{
public:
	VKProgram(const Descriptor& desc);
	explicit VKProgram(const std::string& name);
	virtual ~VKProgram();

	std::vector<VkPipelineShaderStageCreateInfo>& getInfos();

	void use() override;

	void setUniform(const std::string& name, int value) override;
	void setUniform(const std::string& name, float value) override;
	void setUniform(const std::string& name, const Vector2& value) override;
	void setUniform(const std::string& name, const Vector3& value) override;
	void setUniform(const std::string& name, const Vector4& value) override;
	void setUniform(const std::string& name, const Matrix4& value) override;

private:
	void addShader(std::shared_ptr<Shader> shader);

	std::unordered_map<Shader::Stage, std::shared_ptr<VKShader>> shaders;
	std::vector<VkPipelineShaderStageCreateInfo> infos;
};
