// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Node.h"
#include "Pin.h"
#include <imgui_node_editor.h>

using namespace ax::NodeEditor;

namespace ui
{

Node::Node(const std::string& label)
	: Widget(label), nodeId(id)
{
}

void Node::update()
{
	BeginNode(nodeId);
	{
		ImGui::TextUnformatted(label.c_str());
		
		if(ImGui::BeginTable(handle.c_str(), 2, 0, ImVec2(120, 40)))
		{
			for(size_t i = 0; i < inputs.size() || i < outputs.size(); i++)
			{
				ImGui::TableNextRow();
				if(i < inputs.size())
				{
					ImGui::TableSetColumnIndex(0);
					inputs[i].update();
				}
				if(i < outputs.size())
				{
					ImGui::TableSetColumnIndex(1);
					outputs[i].update();
				}
			}
			ImGui::EndTable();
		}
	}
	EndNode();
}

void Node::addPin(const Pin& pin)
{
	if(pin.getKind() == Pin::Kind::Input)
	{
		inputs.push_back(pin);
		inputs.back().setNode(*this);
	}
	else
	{
		outputs.push_back(pin);
		outputs.back().setNode(*this);
	}
}

std::vector<Pin>& Node::getInputs()
{
	return inputs;
}

std::vector<Pin>& Node::getOutputs()
{
	return outputs;
}

} // namespace ui
