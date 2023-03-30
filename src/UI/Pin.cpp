// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Pin.h"
#include <imnodes.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

namespace
{

enum class IconType : ImU32
{
	Flow,
	Circle,
	Square,
	Grid,
	RoundSquare,
	Diamond
};

std::unordered_map<ui::Pin::Type, ImColor> PinIconColor = {
    {ui::Pin::Type::Flow, ImColor(255, 255, 255)},   {ui::Pin::Type::Bool, ImColor(220, 48, 48)},
    {ui::Pin::Type::Int, ImColor(68, 201, 156)},     {ui::Pin::Type::Float, ImColor(147, 226, 74)},
    {ui::Pin::Type::String, ImColor(124, 21, 153)},  {ui::Pin::Type::Object, ImColor(51, 150, 215)},
    {ui::Pin::Type::Function, ImColor(218, 0, 183)}, {ui::Pin::Type::Delegate, ImColor(255, 48, 48)}};

std::unordered_map<ui::Pin::Type, IconType> PinIconType = {
    {ui::Pin::Type::Flow, IconType::Flow},       {ui::Pin::Type::Bool, IconType::Circle},
    {ui::Pin::Type::Int, IconType::Circle},      {ui::Pin::Type::Float, IconType::Circle},
    {ui::Pin::Type::String, IconType::Circle},   {ui::Pin::Type::Object, IconType::Circle},
    {ui::Pin::Type::Function, IconType::Circle}, {ui::Pin::Type::Delegate, IconType::Square}};

void DrawIconFlow(ImDrawList* drawList, const ImRect& rect, bool filled, ImU32 color, ImU32 innerColor)
{
	auto       rect_w        = rect.Max.x - rect.Min.x;
	const auto outline_scale = rect_w / 24.0f;

	const auto origin_scale = rect_w / 24.0f;

	const auto offset_x  = 1.0f * origin_scale;
	const auto offset_y  = 0.0f * origin_scale;
	const auto margin    = (filled ? 2.0f : 2.0f) * origin_scale;
	const auto rounding  = 0.1f * origin_scale;
	const auto tip_round = 0.7f; // percentage of triangle edge (for tip)
	// const auto edge_round = 0.7f; // percentage of triangle edge (for corner)
	const auto canvas   = ImRect(rect.Min.x + margin + offset_x, rect.Min.y + margin + offset_y,
	                             rect.Max.x - margin + offset_x, rect.Max.y - margin + offset_y);
	const auto canvas_x = canvas.Min.x;
	const auto canvas_y = canvas.Min.y;
	const auto canvas_w = canvas.Max.x - canvas.Min.x;
	const auto canvas_h = canvas.Max.y - canvas.Min.y;

	const auto left     = canvas_x + canvas_w * 0.5f * 0.3f;
	const auto right    = canvas_x + canvas_w - canvas_w * 0.5f * 0.3f;
	const auto top      = canvas_y + canvas_h * 0.5f * 0.2f;
	const auto bottom   = canvas_y + canvas_h - canvas_h * 0.5f * 0.2f;
	const auto center_y = (top + bottom) * 0.5f;
	// const auto angle = AX_PI * 0.5f * 0.5f * 0.5f;

	const auto tip_top    = ImVec2(canvas_x + canvas_w * 0.5f, top);
	const auto tip_right  = ImVec2(right, center_y);
	const auto tip_bottom = ImVec2(canvas_x + canvas_w * 0.5f, bottom);

	drawList->PathLineTo(ImVec2(left, top) + ImVec2(0, rounding));
	drawList->PathBezierCurveTo(ImVec2(left, top), ImVec2(left, top), ImVec2(left, top) + ImVec2(rounding, 0));
	drawList->PathLineTo(tip_top);
	drawList->PathLineTo(tip_top + (tip_right - tip_top) * tip_round);
	drawList->PathBezierCurveTo(tip_right, tip_right, tip_bottom + (tip_right - tip_bottom) * tip_round);
	drawList->PathLineTo(tip_bottom);
	drawList->PathLineTo(ImVec2(left, bottom) + ImVec2(rounding, 0));
	drawList->PathBezierCurveTo(ImVec2(left, bottom), ImVec2(left, bottom), ImVec2(left, bottom) - ImVec2(0, rounding));

	if(!filled)
	{
		if(innerColor & 0xFF000000)
			drawList->AddConvexPolyFilled(drawList->_Path.Data, drawList->_Path.Size, innerColor);

		drawList->PathStroke(color, true, 2.0f * outline_scale);
	}
	else
		drawList->PathFillConvex(color);
}

void DrawIconCircle(ImDrawList* drawList, const ImRect& rect, bool filled, ImU32 color, ImU32 innerColor)
{
	auto       rect_w         = rect.Max.x - rect.Min.x;
	auto       rect_center_x  = (rect.Min.x + rect.Max.x) * 0.5f;
	auto       rect_center_y  = (rect.Min.y + rect.Max.y) * 0.5f;
	auto       rect_center    = ImVec2(rect_center_x, rect_center_y);
	const auto outline_scale  = rect_w / 24.0f;
	const auto extra_segments = static_cast<int>(2 * outline_scale); // for full circle

	const auto c = rect_center;

	if(!filled)
	{
		const auto r = 0.5f * rect_w / 2.0f - 0.5f;

		if(innerColor & 0xFF000000)
			drawList->AddCircleFilled(c, r, innerColor, 12 + extra_segments);
		drawList->AddCircle(c, r, color, 12 + extra_segments, 2.0f * outline_scale);
	}
	else
		drawList->AddCircleFilled(c, 0.5f * rect_w / 2.0f, color, 12 + extra_segments);
}

void DrawIconSquare(ImDrawList* drawList, const ImRect& rect, bool filled, ImU32 color, ImU32 innerColor)
{
	auto       rect_w         = rect.Max.x - rect.Min.x;
	auto       rect_center_x  = (rect.Min.x + rect.Max.x) * 0.5f;
	auto       rect_center_y  = (rect.Min.y + rect.Max.y) * 0.5f;
	auto       rect_center    = ImVec2(rect_center_x, rect_center_y);
	const auto outline_scale  = rect_w / 24.0f;
	const auto extra_segments = static_cast<int>(2 * outline_scale); // for full circle

	if(filled)
	{
		const auto r  = 0.5f * rect_w / 2.0f;
		const auto p0 = rect_center - ImVec2(r, r);
		const auto p1 = rect_center + ImVec2(r, r);

		drawList->AddRectFilled(p0, p1, color, 0, 15 + extra_segments);
	}
	else
	{
		const auto r  = 0.5f * rect_w / 2.0f - 0.5f;
		const auto p0 = rect_center - ImVec2(r, r);
		const auto p1 = rect_center + ImVec2(r, r);

		if(innerColor & 0xFF000000)
			drawList->AddRectFilled(p0, p1, innerColor, 0, 15 + extra_segments);

		drawList->AddRect(p0, p1, color, 0, 15 + extra_segments, 2.0f * outline_scale);
	}
}

void drawIconGrid(ImDrawList* drawList, const ImRect& rect, bool filled, ImU32 color, ImU32 innerColor)
{
	auto rect_w        = rect.Max.x - rect.Min.x;
	auto rect_center_x = (rect.Min.x + rect.Max.x) * 0.5f;
	auto rect_center_y = (rect.Min.y + rect.Max.y) * 0.5f;

	const auto r = 0.5f * rect_w / 2.0f;
	const auto w = ceilf(r / 3.0f);

	const auto baseTl = ImVec2(floorf(rect_center_x - w * 2.5f), floorf(rect_center_y - w * 2.5f));
	const auto baseBr = ImVec2(floorf(baseTl.x + w), floorf(baseTl.y + w));

	auto tl = baseTl;
	auto br = baseBr;
	for(int i = 0; i < 3; ++i)
	{
		tl.x = baseTl.x;
		br.x = baseBr.x;
		drawList->AddRectFilled(tl, br, color);
		tl.x += w * 2;
		br.x += w * 2;
		if(i != 1 || filled)
			drawList->AddRectFilled(tl, br, color);
		tl.x += w * 2;
		br.x += w * 2;
		drawList->AddRectFilled(tl, br, color);

		tl.y += w * 2;
		br.y += w * 2;
	}
}

void DrawIconRoundSquare(ImDrawList* drawList, const ImRect& rect, bool filled, ImU32 color, ImU32 innerColor)
{
	auto       rect_w        = rect.Max.x - rect.Min.x;
	auto       rect_center_x = (rect.Min.x + rect.Max.x) * 0.5f;
	auto       rect_center_y = (rect.Min.y + rect.Max.y) * 0.5f;
	auto       rect_center   = ImVec2(rect_center_x, rect_center_y);
	const auto outline_scale = rect_w / 24.0f;

	if(filled)
	{
		const auto r  = 0.5f * rect_w / 2.0f;
		const auto cr = r * 0.5f;
		const auto p0 = rect_center - ImVec2(r, r);
		const auto p1 = rect_center + ImVec2(r, r);

		drawList->AddRectFilled(p0, p1, color, cr, 15);
	}
	else
	{
		const auto r  = 0.5f * rect_w / 2.0f - 0.5f;
		const auto cr = r * 0.5f;
		const auto p0 = rect_center - ImVec2(r, r);
		const auto p1 = rect_center + ImVec2(r, r);

		if(innerColor & 0xFF000000)
			drawList->AddRectFilled(p0, p1, innerColor, cr, 15);

		drawList->AddRect(p0, p1, color, cr, 15, 2.0f * outline_scale);
	}
}

void DrawIconDiamond(ImDrawList* drawList, const ImRect& rect, bool filled, ImU32 color, ImU32 innerColor)
{
	auto       rect_w        = rect.Max.x - rect.Min.x;
	auto       rect_center_x = (rect.Min.x + rect.Max.x) * 0.5f;
	auto       rect_center_y = (rect.Min.y + rect.Max.y) * 0.5f;
	auto       rect_center   = ImVec2(rect_center_x, rect_center_y);
	const auto outline_scale = rect_w / 24.0f;

	if(filled)
	{
		const auto r = 0.607f * rect_w / 2.0f;
		const auto c = rect_center;

		drawList->PathLineTo(c + ImVec2(0, -r));
		drawList->PathLineTo(c + ImVec2(r, 0));
		drawList->PathLineTo(c + ImVec2(0, r));
		drawList->PathLineTo(c + ImVec2(-r, 0));
		drawList->PathFillConvex(color);
	}
	else
	{
		const auto r = 0.607f * rect_w / 2.0f - 0.5f;
		const auto c = rect_center;

		drawList->PathLineTo(c + ImVec2(0, -r));
		drawList->PathLineTo(c + ImVec2(r, 0));
		drawList->PathLineTo(c + ImVec2(0, r));
		drawList->PathLineTo(c + ImVec2(-r, 0));

		if(innerColor & 0xFF000000)
			drawList->AddConvexPolyFilled(drawList->_Path.Data, drawList->_Path.Size, innerColor);

		drawList->PathStroke(color, true, 2.0f * outline_scale);
	}
}

void DrawIcon(ImDrawList* drawList, const ImVec2& a, const ImVec2& b, IconType type, bool filled, ImU32 color,
              ImU32 innerColor)
{
	auto       rect          = ImRect(a, b);
	auto       rect_y        = rect.Min.y;
	auto       rect_w        = rect.Max.x - rect.Min.x;
	auto       rect_h        = rect.Max.y - rect.Min.y;
	auto       rect_center_x = (rect.Min.x + rect.Max.x) * 0.5f;
	auto       rect_center_y = (rect.Min.y + rect.Max.y) * 0.5f;
	auto       triangleStart = rect_center_x + 0.32f * rect_w;
	const auto triangleTip   = triangleStart + rect_w * (0.45f - 0.32f);

	switch(type)
	{
		using enum IconType;

	case Flow:
		DrawIconFlow(drawList, rect, filled, color, innerColor);
		break;

	case Circle:
		DrawIconCircle(drawList, rect, filled, color, innerColor);
		break;

	case Square:
		DrawIconSquare(drawList, rect, filled, color, innerColor);
		break;

	case Grid:
		drawIconGrid(drawList, rect, filled, color, innerColor);
		break;

	case RoundSquare:
		DrawIconRoundSquare(drawList, rect, filled, color, innerColor);
		break;

	case Diamond:
		DrawIconDiamond(drawList, rect, filled, color, innerColor);
		break;

	default:
		drawList->AddTriangleFilled(ImVec2(ceilf(triangleTip), rect_y + rect_h * 0.5f),
		                            ImVec2(triangleStart, rect_center_y + 0.15f * rect_h),
		                            ImVec2(triangleStart, rect_center_y - 0.15f * rect_h), color);
	}
}

void Icon(const ImVec2& size, IconType type, bool filled, const ImVec4& color = ImVec4(1, 1, 1, 1),
          const ImVec4& innerColor = ImVec4(0, 0, 0, 0))
{
	if(ImGui::IsRectVisible(size))
	{
		auto cursorPos = ImGui::GetCursorScreenPos();
		auto drawList  = ImGui::GetWindowDrawList();
		DrawIcon(drawList, cursorPos, cursorPos + size, type, filled, ImColor(color), ImColor(innerColor));
	}
	ImGui::Dummy(size);
}

} // namespace

namespace ui
{

Pin::Pin(const std::string& label, Kind kind, Type type) : Widget(label), kind_(kind), type_(type)
{
}

Pin::Type Pin::get_type() const noexcept
{
	return type_;
}

Pin::Kind Pin::get_kind() const noexcept
{
	return kind_;
}

void Pin::update()
{
	if(kind_ == Kind::Input)
	{
		ImNodes::BeginInputAttribute(id_);

		Icon(ImVec2(16, 16), PinIconType[type_], false, PinIconColor[type_]);
		ImGui::SameLine();
		ImGui::TextUnformatted(label_.c_str());

		ImNodes::EndInputAttribute();
	}
	else
	{
		ImNodes::BeginOutputAttribute(id_);

		// ImGui::Indent(ImGui::GetColumnWidth() - ImGui::CalcTextSize(label.c_str()).x);

		ImGui::TextUnformatted(label_.c_str());
		ImGui::SameLine();
		Icon(ImVec2(16, 16), PinIconType[type_], false, PinIconColor[type_]);

		ImNodes::EndOutputAttribute();
	}
}

} // namespace ui
