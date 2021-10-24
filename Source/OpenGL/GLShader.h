// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "../Shader.h"

class GLShader : public Shader
{
public:
    GLShader(const std::string& name, Stage stage);
    virtual ~GLShader();

    void load();

    size_t getNativeHandle() const override;

private:
    unsigned int handle;
};