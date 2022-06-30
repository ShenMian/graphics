// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Primitive.h"
#include "Mesh.h"
#include <numbers>

// https://winter.dev/prims/
// TODO: 生成 normal

namespace
{

constexpr auto ICO_X = .525731112119133606f;
constexpr auto ICO_Z = .850650808352039932f;

constexpr unsigned int IcoVertCount = 12;
constexpr unsigned int IcoIndexCount = 60;

constexpr Vector3 IcoVerts[] = {
	{0,      -ICO_X, -ICO_Z},
	{-ICO_X, -ICO_Z,      0},
	{ICO_Z,       0, -ICO_X},
	{ICO_X,  -ICO_Z,      0},
	{ICO_X,   ICO_Z,      0},
	{-ICO_X, -ICO_Z,      0},
	{ICO_Z,       0,  ICO_X},
	{0,      -ICO_X,  ICO_Z},
	{ICO_X,   ICO_Z,      0},
	{-ICO_X, -ICO_Z,      0},
	{0,       ICO_X,  ICO_Z},
	{-ICO_Z,      0,  ICO_X},
	{ICO_X,   ICO_Z,      0},
	{-ICO_X, -ICO_Z,      0},
	{-ICO_X,  ICO_Z,      0},
	{-ICO_Z,      0, -ICO_X},
	{ICO_X,   ICO_Z,      0},
	{-ICO_X, -ICO_Z,      0},
	{0,       ICO_X, -ICO_Z},
	{0,      -ICO_X, -ICO_Z},
	{ICO_X,   ICO_Z,      0},
	{ICO_Z,       0, -ICO_X}
};

constexpr Vector2 IcoUvs[] = {
	{0.0,      0.157461},
	{0.090909, 0.0},
	{0.090909, 0.314921},  //
	{0.181818, 0.157461},  // Verts & UVs are ordered by U then Y coords,
	{0.181818, 0.472382},  //
	{0.272727, 0.0},       //      4   8   C   G   K
	{0.272727, 0.314921},  //     / \ / \ / \ / \ / \ 
	{0.363636, 0.157461},  //    2---6---A---E---I---L
	{0.363636, 0.472382},  //   / \ / \ / \ / \ / \ /
	{0.454545, 0.0},       //  0---3---7---B---F---J
	{0.454545, 0.314921},  //   \ / \ / \ / \ / \ /
	{0.545454, 0.157461},  //    1   5   9   D	  H
	{0.545454, 0.472382},  //
	{0.636363, 0.0},       // [4, 8, C, G, K] have the same position vert
	{0.636363, 0.314921},  // [1, 5, 9, D, H] have the same position vert
	{0.727272, 0.157461},  // [0, J]          have the same position vert
	{0.727272, 0.472382},  // [2, L]          have the same position vert
	{0.818181, 0.0},       // 
	{0.818181, 0.314921},
	{0.90909,  0.157461},
	{0.90909,  0.472382},
	{1.0,      0.314921}
};

constexpr unsigned int IcoIndex[] = {
	 0,  1,  3, // Bottom
	 3,  5,  7,
	 7,  9, 11,
	11, 13, 15,
	15, 17, 19,
	 0,  3,  2, // Middle
	 2,  3,  6,
	 3,  7,  6,
	 6,  7, 10,
	 7, 11, 10,
	10, 11, 14,
	11, 15, 14,
	14, 15, 18,
	15, 19, 18,
	18, 19, 21,
	 2,  6,  4, // Top
	 6, 10,  8,
	10, 14, 12,
	14, 18, 16,
	18, 21, 20
};

struct pair_hash {
	template<typename T1, typename T2>
	size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

using IndexPair = std::pair<unsigned, unsigned>;
using IndexLookup = std::unordered_map<IndexPair, unsigned, pair_hash>;

unsigned int CreateVertexForEdge(
	IndexLookup& lookup,
	std::vector<Mesh::Vertex>& vertices,
	unsigned int first,
	unsigned int second,
	unsigned int& currentVertCount)
{
	// Order pair by smallest index first because first, second is equivalient
	// to second, first but the hash is not
	IndexPair edge = first < second ? IndexPair(first, second) : IndexPair(second, first);

	// Check the edge for a vert
	auto inserted = lookup.insert({edge, currentVertCount});

	// If there wasn't one, create a new vert in the middle of the edge
	if(inserted.second && vertices.empty())
	{
		vertices[currentVertCount].position = (vertices[first].position + vertices[second].position) / 2.f;
		vertices[currentVertCount++].uv = (vertices[first].uv + vertices[second].uv) / 2.f;
	}

	// Return the index of the vert
	return inserted.first->second;
}

void SubDevideVerts(
	std::vector<Mesh::Vertex>& vertices,
	std::vector<unsigned int>& index,
	unsigned int& currentIndexCount,
	unsigned int& currentVertCount)
{
	IndexLookup lookup;

	// Next index is going to be the current index * the 16 new indices seen below

	unsigned int* next = new unsigned int[16 * currentIndexCount];
	unsigned int nextCount = 0;

	//       i + 2                    i + 2
	//        / \                      / \
		//       /   \                    /   \
		//      /     \                  /     \
		//     /       \    ------->   mid2---mid1
		//    /         \              / \     / \
		//   /           \            /   \   /   \
		//  /             \          /     \ /     \
		// i-------------i + 1      i-----mid0----i + 1

	for(unsigned int i = 0; i < currentIndexCount; i += 3) {
		unsigned int mid0 = CreateVertexForEdge(
			lookup, vertices, index[i], index[i + ((i + 1) % 3)], currentVertCount);

		unsigned int mid1 = CreateVertexForEdge(
			lookup, vertices, index[i + 1], index[i + ((i + 2) % 3)], currentVertCount);

		unsigned int mid2 = CreateVertexForEdge(
			lookup, vertices, index[i + 2], index[i + ((i + 3) % 3)], currentVertCount);

		next[nextCount++] = index[i];     next[nextCount++] = mid0; next[nextCount++] = mid2;
		next[nextCount++] = index[i + 1]; next[nextCount++] = mid1; next[nextCount++] = mid0;
		next[nextCount++] = index[i + 2]; next[nextCount++] = mid2; next[nextCount++] = mid1;
		next[nextCount++] = mid0;         next[nextCount++] = mid1; next[nextCount++] = mid2;
	}

	// Update index with new index

	memcpy(index.data(), next, nextCount * sizeof(unsigned int));
	currentIndexCount = nextCount;

	delete[] next;
}

}

Mesh Primitive::makePlane(unsigned int x, unsigned int z)
{
	// Edge case throws off counts so return nullptrs
	if(x == 0 || z == 0)
		return Mesh();

	// Each count makes two more triangles (6 indices) for every other count
	// Each count makes its verts + 1 for the final edge
											  // vert  # = (2+1)*(1+1) = 6
											  // index # = 6 * (2)*(1) = 12
											  // •------•------•
	const auto indexCount = 6 * x * z;        // |   /  |   /  |
	const auto vertCount = (x + 1) * (z + 1); // |  /   |  /   |
											  // •------•------•

	std::vector<unsigned int> indices(indexCount);
	std::vector<Mesh::Vertex> vertices(vertCount);

	// Verts

	float xStep = 2.0f / x; // This plane is going to span from -1 to 1
	float zStep = 2.0f / z; // so the step size is 2 / count

	float uStep = 1.0f / x; // UV space spans from 0 to 1 so step size
	float vStep = 1.0f / z; // is 1 / count

	// Translte origin instead of loop variables

	Vector3 offset = Vector3(-1, 0, -1);

	for(unsigned int x_ = 0; x_ <= x; x_++)
	{
		for(unsigned int z_ = 0; z_ <= z; z_++)
		{
			unsigned int i = z_ + x_ * (z + 1);

			// 'Forward' is typically -Z so either the verts or UVs need
			// to be flipped to align textures correctly

			vertices[i].position = Vector3(x_ * xStep, 0, z_ * zStep) + offset;
			vertices[i].uv = Vector2(x_ * uStep, (z - z_) * vStep);
		}
	}

	// Index

	//        --------- x --------->
	//        v          v + 1      (v + 1) + 1
	//    |   •------------•------------•
	//    |   |          / |          / |
	// z |       /    |       /    |
	//    |   |    /       |    /       |
	//    |   | /          | /          |
	//    ↓   •------------•------------•
	//    v + zc + 1   v + zc + 2   (v + 1) + zc + 2

	for(unsigned int v = 0, i = 0; v < vertCount - z - 2; v++)
	{
		// If at the final row, jump to next column

		if((v + 1) % (z + 1) == 0)
			v++;

		indices[i++] = v;
		indices[i++] = v + 1;
		indices[i++] = v + z + 1;

		indices[i++] = v + 1;
		indices[i++] = v + z + 2;
		indices[i++] = v + z + 1;
	}

	return Mesh("plane", vertices, indices);
}

Mesh Primitive::makeSphere(unsigned int lat, unsigned int lon)
{
	// Edge case throws off counts so return nullptrs
	if(lon < 2 || lat < 2)
		return Mesh();

	// Each longitudinal count makes two triangles (6 indices) for every
	// lateral count except for the top and bottom caps, which only make
	// one triangle per lon count. Subtracting one from lat
	// effectivly combines the top and bottom counts.

	// UV maps require two verts with the same position, but different UVs
	// so we need counts + 1.

	const auto indexCount = 6 * lon * (lat - 1);
	const auto vertCount = (lon + 1) * (lat + 1);

	std::vector<unsigned int> indices(indexCount);
	std::vector<Mesh::Vertex> vertices(vertCount);

	// Verts

	float lonStep = std::numbers::pi * 2 / lon; // FIXME: pi2 means pi * 2 or pi ^ 2?
	float latStep = std::numbers::pi / lat;

	for(unsigned int lat_ = 0, v = 0; lat_ <= lat; lat_++) {
		for(unsigned int lon_ = 0; lon_ <= lon; lon_++, v++) {
			vertices[v].position = Vector3(
				cos(lon_ * lonStep) * sin(lat_ * latStep), // Circle equations from UVs
				cos(lat_ * latStep - std::numbers::pi),    // bottom to top
				sin(lon_ * lonStep) * sin(lat_ * latStep)
			);

			vertices[v].uv = Vector2( // This is a UV spehre, so uvs map directly
				(float)lon_ / lon,     // to the verts. This is effectivly a Mercator
				(float)lat_ / lat      // projection
			);
		}
	}

	// Index

	// Top cap
	//
	// One triangle connects the first lateral layer to the second per lon. count
	// Even though the top points all have the same position, their UVs are different
	// so each triangle uses a different point (lon). v = second layer
	//
	//          -------- lon -------->
	//                      lon
	//    |                  •
	//    |                / | \
	//    1             /    |    \
	//    |          /       |       \
	//    |       /                     \
	//    ↓     •------------•------------•
	//          v          v + 1      (v + 1) + 1
	//

	unsigned int i = 0;
	unsigned int v = lon + 1;
	for(unsigned int lon_ = 0; lon_ < lon; lon_++, v++) {
		indices[i++] = lon_;
		indices[i++] = v;
		indices[i++] = v + 1;
	}

	// Middle
	//
	// Each lateral layer has 2 triangles per lon. count (effectivly making quads)
	// Reset v to start of second layer
	//
	//          -------- lon -------->
	//          v          v + 1      (v + 1) + 1
	//    |     •------------•------------•
	//    |     |          / |          / |
	// lat |       /    |       /    |
	//    |     |    /       |    /       |
	//    |     | /          | /          |
	//    ↓     •------------•------------•
	//      v + lc + 1   v + lc + 2   (v + 1) + lc + 2
	//

	v = lon + 1;
	for(unsigned int lat_ = 1; lat_ < lat - 1; lat_++, v++) {
		for(unsigned int lon_ = 0; lon_ < lon; lon_++, v++) {
			indices[i++] = v;
			indices[i++] = v + lon + 1;
			indices[i++] = v + 1;

			indices[i++] = v + 1;
			indices[i++] = v + lon + 1;
			indices[i++] = v + lon + 2;
		}
	}

	// Bottom cap

	// Same as top cap
	// Middle loops work out v to correct value
	//
	//          -------- lon -------->
	//          v          v + 1      (v + 1) + 1
	//    |     •------------•------------•
	//    |       \          |          /
	//    1          \       |       /
	//    |             \    |    /
	//    |                \ | /
	//    ↓                  •
	//                   v + lc + 1
	//

	for(unsigned int lon_ = 0; lon_ < lon; lon_++, v++) {
		indices[i++] = v;
		indices[i++] = v + lon + 1;
		indices[i++] = v + 1;
	}

	return Mesh("sphere", vertices, indices);
}

Mesh Primitive::makeCapsule(unsigned int resolution, float height, float radius)
{
	// Edge case throws off counts so return nullptrs
	if(resolution < 2)
		return Mesh();

	// Almost same generation as UV sphere but we force the
	// lat count to be odd so it can be split evenly

	auto lonCount = resolution;
	auto latCount = resolution;

	if(latCount % 2 == 0)
		latCount++;

	// Each longitudinal count makes two triangles (6 indices) for every
	// lateral count except for the top and bottom caps, which only make
	// one triangle per lon count. Subtracting one from latCount
	// effectivly combines the top and bottom counts.

	// UV maps require two verts with the same position, but different UVs
	// so we need counts + 1.

	const auto indexCount = 6 * lonCount * (latCount - 1);
	const auto vertCount = (lonCount + 1) * (latCount + 1);

	std::vector<unsigned int> indices(indexCount);
	std::vector<Mesh::Vertex> vertices(vertCount);

	// Verts

	float yOffset = (height - radius * 2) * 0.5;
	if(yOffset < 0)
		yOffset = 0; // Limit yOffset to zero

	float lonStep = std::numbers::pi * 2 / lonCount; // FIXME: pi2 means pi * 2 or pi ^ 2?
	float latStep = std::numbers::pi / latCount;
	// float xStep = 1.0f / lonCount;

	for(unsigned int lat = 0, v = 0; lat <= latCount; lat++)
	{
		for(unsigned int lon = 0; lon <= lonCount; lon++, v++)
		{
			vertices[v].position = Vector3(
				cos(lon * lonStep) * sin(lat * latStep), // Circle equations from UVs
				cos(lat * latStep - std::numbers::pi),       // bottom to top
				sin(lon * lonStep) * sin(lat * latStep)
			);

			vertices[v].position *= radius;

			// Move top hemisphere up and bottom down

			if(lat > latCount / 2)
				vertices[v].position.y += yOffset;
			else
				vertices[v].position.y -= yOffset;

			// UVs are almost the same as UV sphere, but V needs to be scaled
			// to fit the height

			vertices[v].uv = Vector2(
				(float)lon / lonCount,
				(vertices[v].position.y + height) * 0.5f / height
			);
		}
	}

	// Index

	// Top cap
	//
	// One triangle connects the first lateral layer to the second per lon. count
	// Even though the top points all have the same position, their UVs are different
	// so each triangle uses a different point (lon). v = second layer
	//
	//          -------- lonCount -------->
	//                      lon
	//    |                  •
	//    |                / | \
	//    1             /    |    \
	//    |          /       |       \
	//    |       /                     \
	//    ↓     •------------•------------•
	//          v          v + 1      (v + 1) + 1
	//

	unsigned int i = 0;
	unsigned int v = lonCount + 1;
	for(unsigned int lon = 0; lon < lonCount; lon++, v++)
	{
		indices[i++] = lon;
		indices[i++] = v;
		indices[i++] = v + 1;
	}

	// Middle
	//
	// Each lateral layer has 2 triangles per lon. count (effectivly making quads)
	// Reset v to start of second layer
	//
	//          -------- lonCount -------->
	//          v          v + 1      (v + 1) + 1
	//    |     •------------•------------•
	//    |     |          / |          / |
	// latCount |       /    |       /    |
	//    |     |    /       |    /       |
	//    |     | /          | /          |
	//    ↓     •------------•------------•
	//      v + lc + 1   v + lc + 2   (v + 1) + lc + 2
	//

	v = lonCount + 1;
	for(unsigned int lat = 1; lat < latCount - 1; lat++, v++)
	{
		for(unsigned int lon = 0; lon < lonCount; lon++, v++)
		{
			indices[i++] = v;
			indices[i++] = v + lonCount + 1;
			indices[i++] = v + 1;

			indices[i++] = v + 1;
			indices[i++] = v + lonCount + 1;
			indices[i++] = v + lonCount + 2;
		}
	}

	// Bottom cap

	// Same as top cap
	// Middle loops work out v to correct value
	//
	//          -------- lonCount -------->
	//          v          v + 1      (v + 1) + 1
	//    |     •------------•------------•
	//    |       \          |          /          
	//    1          \       |       /
	//    |             \    |    /
	//    |                \ | /
	//    ↓                  •
	//                   v + lc + 1
	//

	for(unsigned int lon = 0; lon < lonCount; lon++, v++)
	{
		indices[i++] = v;
		indices[i++] = v + lonCount + 1;
		indices[i++] = v + 1;
	}

	return Mesh("capsule", vertices, indices);
}

Mesh Primitive::makeIcoSphere(unsigned int resolution)
{
	// For every resolution, each face of the initial mesh gets split into 4 triangles.
	// Most of the vertices end up begin shared between many different triangles
	// so the number of vertices works out to a geometric sequence summing 4^R
	//
	//     •-------•               •---•---•
	//    / \     /               / \ / \ /
	//   /   \   /	  ------->    •---•---•
	//  /     \ /               / \ / \ /
	// •-------•               •---•---•

	unsigned int res = (unsigned int)pow(4, resolution);

	const auto indexCount = 60 * res;
	const auto vertCount = 22 + 60 * (1 - res) / -3;

	std::vector<unsigned int> indices(indexCount);
	std::vector<Mesh::Vertex> vertices(vertCount);

	memcpy(indices.data(), IcoIndex, IcoIndexCount * sizeof(unsigned int));

	for(unsigned int i = 0; i < IcoVertCount; i++)
	{
		vertices[i].position = IcoVerts[i];
		vertices[i].uv = IcoUvs[i];
	}

	// Verts & Index

	unsigned int currentIndexCount = IcoIndexCount;
	unsigned int currentVertCount = IcoVertCount;
	for(unsigned int i = 0; i < resolution; i++)
		SubDevideVerts(vertices, indices, currentIndexCount, currentVertCount);

	// At this point we have a tessellated icosahedron, but most of the points don't
	// have length = 1. Normalizing them it will push all but the initial points out
	// creating a sphere 

	for(unsigned int i = 0; i < vertCount; i++)
		vertices[i].position.normalize();

	return Mesh("icosphere", vertices, indices);
}
