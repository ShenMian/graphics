// Copyright 2021 SMS
// License(Apache-2.0)

#include "Scene.h"
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cassert>

namespace fs = std::filesystem;

void Scene::load(const fs::path& path)
{
    Assimp::Importer importer;

    // 从文件导入场景数据
    auto scene = importer.ReadFile(path.string(),
                                   aiProcess_CalcTangentSpace |
                                       aiProcess_Triangulate |
                                       aiProcess_JoinIdenticalVertices |
                                       aiProcess_SortByPType);

    assert(scene != nullptr);
}

void Scene::save(const fs::path& path)
{
    assert(scene != nullptr);

    Assimp::Exporter exporter;

    // 导出场景数据到文件
    exporter.Export(scene, path.extension().string(), path.string());
}
