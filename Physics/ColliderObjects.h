#pragma once

#include "Mesh.h"

#include <glm/glm.hpp>

#include <vector>

struct SphereCollider
{
	SphereCollider(float radius) : radius(radius) {}
	float radius;
};

struct AABBCollider
{
	AABBCollider(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
	glm::vec3 min, max;
};

struct MeshCollider
{
	MeshCollider(std::vector<Vertex> vertices, std::vector<Face> indces) : vertices(vertices), indces(indces) {}

	std::vector<Vertex> vertices;
	std::vector<Face> indces;
};