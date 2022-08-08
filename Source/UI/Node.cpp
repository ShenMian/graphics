// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Node.h"
#include "Pin.h"
#include <imnodes.h>
#include <algorithm>

namespace ui
{

Node::Node(const std::string& label)
	: Widget(label)
{
}

void Node::addPin(const Pin& pin)
{
	inputs.push_back(pin);
}

const Pin* Node::getPinById(uint64_t id)
{
	{
		const auto it = std::ranges::find_if(inputs, [id](const auto& pin) { return pin.getId() == id; });
		if(it != inputs.end())
			return &*it;
	}

	{
		const auto it = std::ranges::find_if(outputs, [id](const auto& pin) { return pin.getId() == id; });
		if(it != outputs.end())
			return &*it;
	}

	return nullptr;
}

void Node::update()
{
	ImNodes::BeginNode(id);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(label.c_str());
	ImNodes::EndNodeTitleBar();

	for(size_t i = 0; i < inputs.size(); i++)
		inputs[i].update();

	for(size_t i = 0; i < outputs.size(); i++)
		outputs[i].update();

	ImNodes::EndNode();
}

} // namespace ui
