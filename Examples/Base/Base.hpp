// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/color.h>
#include <fmt/format.h>

class Base
{
public:
	virtual int main(int argc, char* argv[]) = 0;

	Base(std::string_view name) : name(name) { init(); }

	virtual ~Base() { deinit(); }

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
			getchar();
			return 1;
		}
	}

	void printMonitorInfo()
	{
		puts("Monitor");
		const auto monitors = Monitor::getMonitors();
		if(monitors.empty())
		{
			printWarn("no monitor");
			return;
		}
		for(size_t i = 0; i < monitors.size() - 1; i++)
		{
			fmt::print("|-{}\n"
			           "| |-Size        : {}x{}\n"
			           "| `-Refresh rate: {} Hz\n",
			           monitors[i].getName(), monitors[i].getSize().x, monitors[i].getSize().y,
			           monitors[i].getRefreshRate());
		}
		fmt::print("`-{}\n"
		           "  |-Size        : {}x{}\n"
		           "  `-Refresh rate: {} Hz\n",
		           monitors.back().getName(), monitors.back().getSize().x, monitors.back().getSize().y,
		           monitors.back().getRefreshRate());
	}

	void printRendererInfo()
	{
		const auto renderer = Renderer::get();
		fmt::print("Renderer\n"
		           "|-Device  : {}\n"
		           "|-Renderer: {}\n"
		           "`-Vendor  : {}\n",
		           renderer->getDeviceName(), renderer->getRendererName(), renderer->getVendorName());
	}

	void printModelInfo(const Model& model)
	{
		fmt::print("Model\n"
		           "|-Name     : {}\n"
		           "|-Triangles: {}\n"
		           "|-Vertices : {}\n"
		           "|-Indices  : {}\n"
		           "|-Meshes   : {}\n"
		           "|-Materials: {}\n"
		           "`-Anims    : {}\n",
		           model.name, model.indexCount / 3, model.vertexCount, model.indexCount, model.meshes.size(),
		           model.materials.size(), model.animations.size());
	}

	void printInfo(std::string_view msg) { fmt::print("{} {}\n", fmt::styled("[*]", fg(fmt::color::blue)), msg); }

	void printGood(std::string_view msg) { fmt::print("{} {}\n", fmt::styled("[+]", fg(fmt::color::red)), msg); }

	void printError(std::string_view msg) { fmt::print("{} {}\n", fmt::styled("[-]", fg(fmt::color::red)), msg); }

	void printWarn(std::string_view msg) { fmt::print("{} {}\n", fmt::styled("[!]", fg(fmt::color::yellow)), msg); }

protected:
	Window* window;

private:
	std::string name;
};
