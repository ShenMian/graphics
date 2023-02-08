// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Widget.h"

namespace ui
{

class Pin;

/**
 * @brief 连接大头针的线.
 */
class Link : public Widget
{
public:
	Link(const Pin& start, const Pin& end);

	const Pin& getStart() const noexcept;
	const Pin& getEnd() const noexcept;

private:
	const Pin& start;
	const Pin& end;
};

}; // namespace ui
