// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

class Renderer
{
public:
    enum class API
    {
        OpenGL,
        Vulkan,
        D3D12,
        Metal
    };

    /**
	 * @brief 设置图形 API.
	 *
	 * @param api 图形 API.
	 */
    static void setAPI(API api);

    /**
	 * @brief 获取图形 API.
	 */
    static API getAPI();

private:
    static API api;
};

// TODO: 移动到合适的位置, 这是与 OpenGL 相关的代码.

void GLCheckError();
void GLClearError();

#if 1
#    define GLCall(func)    \
        do                  \
        {                   \
            GLClearError(); \
            func;           \
            GLCheckError(); \
        } while(false)
#else
#    define GLCall(func) func
#endif