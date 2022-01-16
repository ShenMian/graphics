// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <imgui_node_editor.h>

using ax::NodeEditor::PinId;

namespace ui
{

class Node;

class Pin : public Widget
{
public:
	enum class Type
	{
		Flow,
		Bool,
		Int,
		Float,
		String,
		Object,
		Function,
		Delegate,
	};

	enum class Kind
	{
		Output,
		Input
	};

	Pin(const std::string& label, Type type, Kind kind);

	Type getType() const;
	Kind getKind() const;

	void setNode(Node& node);
	Node* getNode() const;

	void update() override;

	operator PinId() const;

	bool connected = false;

private:
	PinId pinId;
	Type  type;
	Kind  kind;
	Node* node = nullptr;

	const float iconSize = 24;
};

} // namespace ui
