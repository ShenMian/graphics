// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Camera.h"
#include "CommandBuffer.h"
#include "CommandQueue.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "PipelineLayout.h"
#include "Program.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"

#include "AssimpImporter.h"
#include "Model.h"
#include "Primitive.h"

#include "Core/Clock.hpp"
#include "Core/Controller.h"
#include "Core/Gamepad.h"
#include "Core/Image.h"
#include "Core/Input.h"
#include "Core/Monitor.h"
#include "Core/Platform.h"
#include "Core/RID.hpp"
#include "Core/Window.h"

#include "UI/Button.h"
#include "UI/Label.h"
#include "UI/Menu.h"
#include "UI/NodeEditor.h"
#include "UI/UI.h"
#include "UI/Window.h"

/**
 * @defgroup core 核心
 * @brief 较为基础的部分, 包括: 图像导入/导出/处理, 输入/输出等.
 *
 * @defgroup model 模型
 * @brief 3D 模型相关. 包括: 模型导入/导出, 网格处理, 纹理材质組等.
 *
 * @defgroup shader 着色器
 * @brief 着色器相关, 包括: 着色器程序, 着色器阶段等.
 */
