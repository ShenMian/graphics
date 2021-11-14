// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Renderer.h"

class VKRenderer : public Renderer
{
public:
    std::string getDeviceName() const override;
    std::string getRendererName() const override;
    std::string getVendorName() const override;
};