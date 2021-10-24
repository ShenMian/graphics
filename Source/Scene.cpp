// Copyright 2021 SMS
// License(Apache-2.0)

#include "Scene.h"
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cassert>
#include <future>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

void Model::load(const fs::path& path, std::error_code& ec)
{
	Assimp::Importer importer;

	// 从文件导入场景数据
	auto scene = importer.ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if(scene == nullptr)
	{
		ec = std::make_error_code(std::errc::no_such_file_or_directory);
		return;
	}

	// 加载网格
	for(unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
	}

	// 加载材质
	for(unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		auto mat = scene->mMaterials[i];
	}

	ec = std::error_code();
}

void Model::loadAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback)
{
	static auto future = std::async([=]()
	{
		std::error_code ec;
		load(path, ec);
		callback(ec);
	});
}

// static std::vector<std::future<void>> futures;
// futures.erase(std::remove_if(futures.begin(), futures.end(), [](std::future<void>& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }), futures.end());

void Model::save(const fs::path& path, std::error_code& ec = ErrorCode)
{
	assert(scene != nullptr);

	Assimp::Exporter exporter;

	// 导出场景数据到文件
	exporter.Export(scene, path.extension().string(), path.string());

	ec = std::error_code();
}

void Model::saveAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback)
{
	static auto future = std::async([=]()
	{
		std::error_code ec;
		save(path, ec);
		callback(ec);
	});
}
