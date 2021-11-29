// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Program.h"
#include <unordered_map>

class Shader;

class GLProgram : public Program
{
public:
	using handle_type = unsigned int;

	explicit GLProgram(const std::string& name);
	virtual ~GLProgram();

	void use() override;

	void setUniform(const std::string& name, int value) override;
	void setUniform(const std::string& name, float value) override;
	// void setUniform(const std::string& name, const Vector2& value) override;
	// void setUniform(const std::string& name, const Vector3& value) override;
	// void setUniform(const std::string& name, const Vector4& value) override;
	// void setUniform(const std::string& name, const Matrix4& value) override;

private:
	int  getUniformLocation(const std::string& name);
	void attach(const std::shared_ptr<Shader> shader);
	void link();

	handle_type                          handle;
	std::unordered_map<std::string, int> uniformLocations;
};
