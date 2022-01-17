// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include "Node.h"
#include "Pin.h"
#include "Link.h"
#include <imgui_node_editor.h>

using ax::NodeEditor::PinId;
using ax::NodeEditor::EditorContext;

namespace ui
{

class NodeEditor : public Widget
{
public:
	NodeEditor(const std::string& label);
	virtual ~NodeEditor();

	void update() override;

	std::vector<Node> nodes;
	std::vector<Link> links;

private:
	void updateCreate();
	void updateDelete();
	Pin* getPin(PinId id);

	EditorContext* context;
};

} // namespace ui
