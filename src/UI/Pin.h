// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Widget.h"

namespace ui
{

class Node;

/**
 * @brief 节点上的大头针.
 */
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

	Pin(const std::string& label, Kind kind, Type type);

	Type getType() const noexcept;
	Kind getKind() const noexcept;

	void update() override;

private:
	Kind kind;
	Type type;
};

} // namespace ui
