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
		const auto monitors = Monitor::getMonitors();
		fmt::print("Monitors\n");
		for(const auto& mon : monitors)
		{
			fmt::print("{:{}}{}\n", "", 2, mon.getName());
			fmt::print("{:{}}{:12}: {}x{}\n", "", 4, "size", mon.getSize().x, mon.getSize().y);
			fmt::print("{:{}}{:12}: {}\n", "", 4, "refresh rate", mon.getRefreshRate());
		}
	}

	void printRendererInfo()
	{
		const auto renderer = Renderer::get();
		fmt::print("Renderer\n");
		fmt::print("{:{}}{:8}: {}\n", "", 2, "device", renderer->getDeviceName());
		fmt::print("{:{}}{:8}: {}\n", "", 2, "renderer", renderer->getRendererName());
		fmt::print("{:{}}{:8}: {}\n", "", 2, "vendor", renderer->getVendorName());
	}

	void printModelInfo(const Model& model)
	{
		fmt::print("Model\n");
		fmt::print("{:{}}{:9}: {}\n", "", 2, "name", model.name);
		fmt::print("{:{}}{:9}: {}\n", "", 2, "triangles", model.indexCount / 3);
		fmt::print("{:{}}{:9}: {}\n", "", 2, "vertices", model.vertexCount);
		fmt::print("{:{}}{:9}: {}\n", "", 2, "indices", model.indexCount);
		fmt::print("{:{}}{:9}: {}\n", "", 2, "meshes", model.meshes.size());
		fmt::print("{:{}}{:9}: {}\n", "", 2, "materials", model.materials.size());
		fmt::print("{:{}}{:9}: {}\n", "", 2, "anims", model.animations.size());
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
