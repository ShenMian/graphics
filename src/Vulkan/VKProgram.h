// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include "Shader.h"
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

class Shader;
class VKShader;

class VKProgram : public Program
{
public:
	VKProgram(const Descriptor& desc);
	virtual ~VKProgram();

	std::vector<VkPipelineShaderStageCreateInfo>& getInfos();

	void use() override;

	void set_uniform(const std::string& name, int value) override;
	void set_uniform(const std::string& name, float value) override;
	void set_uniform(const std::string& name, const Vector2& value) override;
	void set_uniform(const std::string& name, const Vector3& value) override;
	void set_uniform(const std::string& name, const Vector4& value) override;
	void set_uniform(const std::string& name, const Matrix4& value) override;

private:
	void add_shader(std::shared_ptr<Shader> shader);

	std::unordered_map<Shader::Stage, std::shared_ptr<VKShader>> shaders_;
	std::vector<VkPipelineShaderStageCreateInfo>                 infos_;
};
