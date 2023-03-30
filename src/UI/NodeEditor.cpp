// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "NodeEditor.h"
#include <algorithm>
#include <imnodes.h>
#include <imnodes_internal.h>

#include "Node.h"
#include "Pin.h"

struct Link
{
	const ui::Pin& start;
	const ui::Pin& end;
};

namespace ui
{

NodeEditor::NodeEditor()
{
	ImNodes::CreateContext();
	// ImNodes::GetCurrentContext()->Style.PinOffset = 10;
	ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

	context_ = ImNodes::EditorContextCreate();

	Pin aPin("A", Pin::Kind::Input, Pin::Type::Bool);
	Pin bPin("B", Pin::Kind::Output, Pin::Type::Int);
	Pin cPin("C", Pin::Kind::Output, Pin::Type::Bool);
	Pin dPin("D", Pin::Kind::Input, Pin::Type::Bool);
	Pin ePin("E", Pin::Kind::Input, Pin::Type::Int);

	Node aNode("A");
	aNode.add_pin(aPin);
	aNode.add_pin(bPin);
	aNode.add_pin(cPin);

	Node bNode("B");
	bNode.add_pin(dPin);
	bNode.add_pin(ePin);

	nodes_.push_back(std::move(aNode));
	nodes_.push_back(std::move(bNode));
}

NodeEditor::~NodeEditor()
{
	ImNodes::DestroyContext();
}

void NodeEditor::update()
{
	ImGui::Begin("node editor");
	ImNodes::BeginNodeEditor();

	for(auto& node : nodes_)
		node.update();

	static std::unordered_map<uint64_t, Link> links;

	for(auto& [id, link] : links)
		ImNodes::Link(id, link.start.get_id(), link.end.get_id());

	// ImNodes::MiniMap();
	ImNodes::EndNodeEditor();
	ImGui::End();

	int startId, endId;
	if(ImNodes::IsLinkCreated(&startId, &endId))
	{
		const Pin* start = nullptr;
		const Pin* end   = nullptr;
		for(auto& node : nodes_)
			if((start = node.get_pin_by_id(startId)))
				break;
		for(auto& node : nodes_)
			if((end = node.get_pin_by_id(endId)))
				break;

		if(start != nullptr && end != nullptr && start->get_kind() != end->get_kind() &&
		   start->get_type() == end->get_type())
			links.insert(std::make_pair(Widget::request_id(), Link{*start, *end}));
	}

	int linkId;
	if(ImNodes::IsLinkDestroyed(&linkId))
	{
		links.erase(linkId);
	}
}

} // namespace ui
