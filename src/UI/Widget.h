// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <functional>
#include <string>
#include <vector>

namespace ui
{

class Widget
{
public:
	Widget();
	Widget(const std::string& label);
	Widget(const std::string& label, const std::string& id);

	Widget(const Widget&);

	/**
	 * @brief 设置组件标签.
	 *
	 * @param label 组件标签. 不能包含字符串 "##".
	 */
	void set_label(const std::string& label);

	/**
	 * @brief 获取组件标签.
	 */
	const std::string& get_label() const noexcept;

	uint64_t get_id() const noexcept;

	virtual void update();

	static uint64_t request_id();

protected:
	/**
	 * @brief 更新句柄.
	 *
	 * 每当 label 或 id 发生改变时, 句柄应该被更新.
	 */
	void update_handle();

	std::string handle_;

	std::string label_;
	uint64_t    id_;

private:
	static uint64_t next_id_;
};

} // namespace ui
