// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "NodeEditor.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

using namespace ax::NodeEditor;

namespace ui
{

NodeEditor::NodeEditor(const std::string& label)
	: Widget(label)
{
	context = CreateEditor();
}

NodeEditor::~NodeEditor()
{
	DestroyEditor(reinterpret_cast<EditorContext*>(context));
}

void NodeEditor::update()
{
	SetCurrentEditor(reinterpret_cast<EditorContext*>(context));

	Begin(handle.c_str());
	{
		for(auto& node : nodes)
			node.update();
		for(auto& link : links)
			link.update();
		updateCreate();
		updateDelete();

		if(ImGui::BeginPopup("Create New Node"))
		{
			// TODO: 新建节点的菜单, 包含若干 ImGui::MenuItem
			ImGui::EndPopup();
		}
	}
	End();
}

void NodeEditor::updateCreate()
{
	if(BeginCreate(ImColor(255, 255, 255), 2.0f))
	{
		// 创建链接
		PinId startPinId = 0, endPinId = 0;
		if(QueryNewLink(&startPinId, &endPinId) && startPinId && endPinId)
		{
			auto start = getPin(startPinId);
			auto end = getPin(endPinId);
			if(start->connected || end->connected)
				RejectNewItem(ImColor(255, 0, 0), 2.0f);
			else if(start->getKind() == end->getKind())
				RejectNewItem(ImColor(255, 0, 0), 2.0f);
			else if(start->getType() != end->getType())
				RejectNewItem(ImColor(255, 0, 0), 2.0f);
			else if(AcceptNewItem(ImColor(128, 255, 128), 4.0f))
				links.emplace_back(*start, *end);
		}

		// 创建节点
		PinId pinId = 0;
		if(QueryNewNode(&pinId) && pinId)
		{
			auto pin = getPin(pinId);

			if(AcceptNewItem())
			{
				Suspend();
				ImGui::OpenPopup("Create New Node");
				Resume();
			}
		}
	}
	EndCreate();
}

void NodeEditor::updateDelete()
{
	if(BeginDelete())
	{
		// 删除链接
		LinkId linkId;
		while(QueryDeletedLink(&linkId))
		{
			if(AcceptDeletedItem())
			{
				// TODO: 从 links 中删除 linkId
			}
		}

		// 删除节点
		NodeId nodeId = 0;
		while(QueryDeletedNode(&nodeId))
		{
			if(AcceptDeletedItem())
			{
				// TODO: 从 nodes 中删除 nodeId
			}
		}
	}
	EndDelete();
}

Pin* NodeEditor::getPin(PinId id)
{
	for(auto& node : nodes)
	{
		for(auto& pin : node.getInputs())
			if(static_cast<PinId>(pin) == id)
				return &pin;
		for(auto& pin : node.getOutputs())
			if(static_cast<PinId>(pin) == id)
				return &pin;
	}
	return nullptr;
}

} // namespace ui
