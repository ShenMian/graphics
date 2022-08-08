// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <vector>
#include <unordered_map>

struct ImNodesEditorContext;

namespace ui
{

class Node;

class NodeEditor
{
public:
	NodeEditor();
	virtual ~NodeEditor();

	void update();

private:
	ImNodesEditorContext* context;
	std::vector<Node> nodes;
};

} // namespace ui
