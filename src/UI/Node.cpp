// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Node.h"
#include "Pin.h"
#include <algorithm>
#include <imnodes.h>

namespace ui
{

Node::Node(const std::string& label) : Widget(label)
{
}

void Node::add_pin(const Pin& pin)
{
	inputs_.push_back(pin);
}

const Pin* Node::get_pin_by_id(uint64_t id)
{
	{
		const auto it = std::ranges::find_if(inputs_, [id](const auto& pin) { return pin.get_id() == id; });
		if(it != inputs_.end())
			return &*it;
	}

	{
		const auto it = std::ranges::find_if(outputs_, [id](const auto& pin) { return pin.get_id() == id; });
		if(it != outputs_.end())
			return &*it;
	}

	return nullptr;
}

void Node::update()
{
	ImNodes::BeginNode(id_);

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(label_.c_str());
	ImNodes::EndNodeTitleBar();

	for(size_t i = 0; i < inputs_.size(); i++)
		inputs_[i].update();

	for(size_t i = 0; i < outputs_.size(); i++)
		outputs_[i].update();

	ImNodes::EndNode();
}

} // namespace ui
