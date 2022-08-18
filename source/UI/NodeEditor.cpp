// Copyright 2021 ShenMian
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

	context = ImNodes::EditorContextCreate();

	Pin aPin("A", Pin::Kind::Input, Pin::Type::Bool);
	Pin bPin("B", Pin::Kind::Output, Pin::Type::Int);
	Pin cPin("C", Pin::Kind::Output, Pin::Type::Bool);
	Pin dPin("D", Pin::Kind::Input, Pin::Type::Bool);
	Pin ePin("E", Pin::Kind::Input, Pin::Type::Int);

	Node aNode("A");
	aNode.addPin(aPin);
	aNode.addPin(bPin);
	aNode.addPin(cPin);

	Node bNode("B");
	bNode.addPin(dPin);
	bNode.addPin(ePin);

	nodes.push_back(std::move(aNode));
	nodes.push_back(std::move(bNode));
}

NodeEditor::~NodeEditor()
{
	ImNodes::DestroyContext();
}

void NodeEditor::update()
{
	ImGui::Begin("node editor");
	ImNodes::BeginNodeEditor();

	for(auto& node : nodes)
		node.update();

	static std::unordered_map<int, Link> links;

	for(auto& [id, link] : links)
		ImNodes::Link(id, link.start.getId(), link.end.getId());

	// ImNodes::MiniMap();
	ImNodes::EndNodeEditor();
	ImGui::End();

	int startId, endId;
	if(ImNodes::IsLinkCreated(&startId, &endId))
	{
		const Pin* start = nullptr;
		const Pin* end   = nullptr;
		for(auto& node : nodes)
			if(start = node.getPinById(startId))
				break;
		for(auto& node : nodes)
			if(end = node.getPinById(endId))
				break;

		if(start != nullptr && end != nullptr && start->getKind() != end->getKind() &&
		   start->getType() == end->getType())
			links.insert(std::make_pair(Widget::requestId(), Link{*start, *end}));
	}

	int linkId;
	if(ImNodes::IsLinkDestroyed(&linkId))
	{
		links.erase(linkId);
	}
}

} // namespace ui
