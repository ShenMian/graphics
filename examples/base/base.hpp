// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Graphics.h"
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/color.h>
#include <fmt/format.h>

namespace fs = std::filesystem;

class Base
{
public:
	virtual int main(int argc, char* argv[]) = 0;

	Base(std::string_view name) : name(name) { init(); }

	virtual ~Base() { deinit(); }

	void init()
	{
		Renderer::set_backend(Renderer::Backend::OpenGL);
		Window::init();
		window = new Window(name, Monitor::get_primary()->get_size() / 2);

		Renderer::init(*window);
		UI::init(*window);
		Font::init();

		Input::set_window(*window);

		print_monitor_info();
		print_renderer_info();
	}

	void deinit()
	{
		Font::deinit();
		UI::deinit();
		Renderer::deinit();

		delete window;
		Window::deinit();
	}

	int run(int argc, char* argv[])
	{
		fs::current_path(fs::path(argv[0]).parent_path());

		try
		{
			return main(argc, argv);
		}
		catch(std::runtime_error& e)
		{
			std::cout << e.what() << '\n';
			getchar();
			return 1;
		}
	}

	void print_monitor_info()
	{
		const auto monitors = Monitor::get_monitors();
		fmt::print("Monitors\n");
		for(const auto& mon : monitors)
		{
			fmt::print("{:{}}{}\n", "", 2, mon.get_name());
			fmt::print("{:{}}{:12}: {}x{}\n", "", 4, "size", mon.get_size().x(), mon.get_size().y());
			fmt::print("{:{}}{:12}: {}\n", "", 4, "refresh rate", mon.get_refresh_rate());
		}
	}

	void print_renderer_info()
	{
		const auto renderer = Renderer::get();
		fmt::print("Renderer\n");
		fmt::print("{:{}}{:8}: {}\n", "", 2, "device", renderer->get_device_name());
		fmt::print("{:{}}{:8}: {}\n", "", 2, "renderer", renderer->get_renderer_name());
		fmt::print("{:{}}{:8}: {}\n", "", 2, "vendor", renderer->get_vendor_name());
	}

	void print_model_info(const Model& model)
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

protected:
	Window* window;

private:
	std::string name;
};
