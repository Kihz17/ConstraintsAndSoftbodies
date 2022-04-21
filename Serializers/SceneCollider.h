#pragma once

#include "Mesh.h"

#include <yaml-cpp/yaml.h>

enum class ColliderType
{
	None = 0,
	Sphere,
	AABB,
	Mesh
};

class SceneCollider
{
public:
	SceneCollider(Ref<Mesh> mesh, float bounciness = 0.1f);
	SceneCollider(ColliderType colliderType, Ref<void> collider, float bounciness);
	~SceneCollider() = default;

	virtual void Save(YAML::Emitter & emitter) const;
	static SceneCollider StaticLoad(Ref<Mesh> mesh, const YAML::Node & node);

	ColliderType colliderType;
	Ref<void> collider;
	float bounciness;
};