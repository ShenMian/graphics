// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Link.h"
#include <imgui_node_editor.h>

namespace ui
{

Link::Link(Pin& start, Pin& end)
	: start(start), end(end)
{
	start.connected = true;
	end.connected = true;
}

void Link::update()
{
	ax::NodeEditor::Link(linkId, start, end);
}

Link::operator LinkId() const
{
	return linkId;
}

} // namespace ui
