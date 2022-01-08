// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"

namespace ui
{

class Window : public Widget
{
public:
	enum Flag
	{
		None = 0,
		NoTitleBar = 1 << 0,                 // 禁用标题栏
		NoResize = 1 << 1,                   // 禁止用户通过拖拽右下角调整大小
		NoMove = 1 << 2,                     // 禁止用户移动窗口
		NoScrollbar = 1 << 3,                // 禁用滚动条 (窗口依然可以通过鼠标或编程的方式滚动)
		NoScrollWithMouse = 1 << 4,          // 禁止用户使用鼠标滚轮垂直滚动. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
		NoCollapse = 1 << 5,                 // 禁止用户通过双击折叠窗口. Also referred to as Window Menu Button (e.g. within a docking node).
		AlwaysAutoResize = 1 << 6,           // Resize every window to its content every frame
		NoBackground = 1 << 7,               // 禁用背景色(WindowBg, 等.)和外边框的绘制. 类似使用 SetNextWindowBgAlpha(0.0f).
		NoSavedSettings = 1 << 8,            // 不从 .ini 文件中导入/导出配置
		NoMouseInputs = 1 << 9,              // Disable catching mouse, hovering test with pass through.
		MenuBar = 1 << 10,                   // Has a menu-bar
		HorizontalScrollbar = 1 << 11,       // 允许水平滚动条显示(默认关). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
		NoFocusOnAppearing = 1 << 12,        // Disable taking focus when transitioning from hidden to visible state
		NoBringToFrontOnFocus = 1 << 13,     // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
		AlwaysVerticalScrollbar = 1 << 14,   // 总是显示垂直滚动条 (即使 ContentSize.y < Size.y)
		AlwaysHorizontalScrollbar = 1 << 15, // 总是显示水平滚动条 (即使 ContentSize.x < Size.x)
		AlwaysUseWindowPadding = 1 << 16,    // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
		NoNavInputs = 1 << 18,               // No gamepad/keyboard navigation within the window
		NoNavFocus = 1 << 19,                // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
		UnsavedDocument = 1 << 20,           // 在标题旁显示一个点. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
		NoDocking = 1 << 21,                 // 禁用该窗口的停靠

		NoNav = NoNavInputs | NoNavFocus,
		NoDecoration = NoTitleBar | NoResize | NoScrollbar | NoCollapse,
		NoInputs = NoMouseInputs | NoNavInputs | NoNavFocus,
	};

	Window(const std::string& label, Flag flags = Flag::None);

	void add(Widget& widget);

	void clear();

	void update() override;

	std::function<void(Window&)> hover;

private:
	Flag                 flags;
	std::vector<Widget*> widgets;
};

} // namespace ui
