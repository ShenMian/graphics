// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Renderer.h"
#include "CommandBuffer.h"
#include "CommandQueue.h"
#include "Texture.h"
#include "VertexFormat.h"

#include "Program.h"
#include "Shader.h"

#include "Model.h"

#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Gamepad.h"
#include "Core/Monitor.h"
#include "Core/Image.h"
#include "Core/Timer.hpp"

// Doxygen main page
/**
 * @mainpage Graphics Documentation
 *
 * Graphics
 * ========
 *
 * Overview
 * --------
 * A simple abstraction layer for the modern graphics APIs.
 *
 * Examples
 * --------
 * [Triangle](https://github.com/ShenMian/Graphics/tree/main/Examples/Triangle)
 *
 * @defgroup core   核心
 * @defgroup model  模型
 * @defgroup shader 渲染器
 */
