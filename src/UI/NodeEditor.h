// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <unordered_map>
#include <vector>

struct ImNodesEditorContext;

namespace ui
{

class Node;

/**
 * @brief 节点编辑器.
 */
class NodeEditor
{
public:
	NodeEditor();
	virtual ~NodeEditor();

	void update();

private:
	ImNodesEditorContext* context;
	std::vector<Node>     nodes;
};

} // namespace ui
