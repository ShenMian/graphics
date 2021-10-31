// Copyright 2021 SMS
// License(Apache-2.0)

#include "CommandQueue.h"

#include "OpenGL/GLCommandQueue.h"

std::shared_ptr<CommandQueue> CommandQueue::create()
{
    return std::make_shared<GLCommandQueue>();
}