// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Widget.h"
#include <vector>

namespace ui
{

class Pin;

/**
 * @brief 节点.
 */
class Node : public Widget
{
public:
	Node(const std::string& label);

	void       addPin(const Pin& pin);
	const Pin* getPinById(uint64_t id);

	void update() override;

private:
	std::vector<Pin> inputs;
	std::vector<Pin> outputs;
};

} // namespace ui
