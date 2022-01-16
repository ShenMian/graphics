// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <imgui_node_editor.h>

using ax::NodeEditor::NodeId;

namespace ui
{

class Pin;

class Node : public Widget
{
public:
	Node(const std::string& label);

	void update() override;

	void addPin(const Pin& pin);

	std::vector<Pin>& getInputs();
	std::vector<Pin>& getOutputs();

private:
	NodeId nodeId;
	std::vector<Pin> inputs;
	std::vector<Pin> outputs;
};

} // namespace ui
