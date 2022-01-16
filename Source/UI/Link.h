// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include "Pin.h"
#include <imgui_node_editor.h>

using ax::NodeEditor::LinkId;

namespace ui
{

class Link : public Widget
{
public:
	Link(Pin& start, Pin& end);

	void update() override;

	operator LinkId() const;

private:
	LinkId linkId;

	Pin start;
	Pin end;
};

} // namespace ui
