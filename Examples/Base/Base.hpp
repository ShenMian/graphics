// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <fmt/color.h>

class Base
{
public:
	virtual int main(int argc, char* argv[]) = 0;

	Base(std::string_view name)
		: name(name)
	{
		init();
	}

	virtual ~Base()
	{
		deinit();
	}

	void init()
	{
		Renderer::setAPI(Renderer::API::OpenGL);
		Window::init();
		window = new Window(name, Monitor::getPrimary()->getSize() / 2);

		Renderer::init(*window);
		UI::init(*window);
		Input::setWindow(*window);

		printMonitorInfo();
		printRendererInfo();
	}

	void deinit()
	{
		UI::deinit();
		Renderer::deinit();

		delete window;
		Window::deinit();
	}

	int run(int argc, char* argv[])
	{
		std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());

		try
		{
			return main(argc, argv);
		}
		catch(std::runtime_error& e)
		{
			printError(e.what());
			return 1;
		}
	}

	void printMonitorInfo()
	{
		puts("Monitor");
		const auto monitors = Monitor::getMonitors();
		if(monitors.empty())
		{
			printWarn("No monitor");
			return;
		}
		for(size_t i = 0; i < monitors.size() - 1; i++)
		{
			puts(fmt::format(
				"|-{}\n"
				"| |-Size        : {}x{}\n"
				"| `-Refresh rate: {} Hz",
				monitors[i].getName(), monitors[i].getSize().x, monitors[i].getSize().y, monitors[i].getRefreshRate()).c_str());
		}
		puts(fmt::format(
			"`-{}\n"
			"  |-Size        : {}x{}\n"
			"  `-Refresh rate: {} Hz",
			monitors.back().getName(), monitors.back().getSize().x, monitors.back().getSize().y, monitors.back().getRefreshRate()).c_str());
	}

	void printRendererInfo()
	{
		const auto renderer = Renderer::get();
		puts(fmt::format(
			"Renderer\n"
			"|-Device  : {}\n"
			"|-Renderer: {}\n"
			"`-Vendor  : {}",
			renderer->getDeviceName(), renderer->getRendererName(), renderer->getVendorName()).c_str());
	}

	void printModelInfo(const Model& model)
	{
		const auto& info = model.getMeshInfo();
		puts(fmt::format(
			"Mesh\n"
			"|-Name     : {}\n"
			"|-Triangles: {}\n"
			"|-Vertices : {}\n"
			"`-Indices  : {}",
			model.getName(), info.triangles, info.vertices, info.indices).c_str());
	}

	void printInfo(std::string_view msg)
	{
		puts(fmt::format("{} {}", fmt::styled("[*]", fg(fmt::color::blue)), msg).c_str());
	}

	void printGood(std::string_view msg)
	{
		puts(fmt::format("{} {}", fmt::styled("[+]", fg(fmt::color::red)), msg).c_str());
	}

	void printError(std::string_view msg)
	{
		puts(fmt::format("{} {}", fmt::styled("[-]", fg(fmt::color::red)), msg).c_str());
	}

	void printWarn(std::string_view msg)
	{
		puts(fmt::format("{} {}", fmt::styled("[!]", fg(fmt::color::yellow)), msg).c_str());
	}

protected:
	Window* window;

private:
	std::string name;
};
