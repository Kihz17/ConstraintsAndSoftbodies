#include "SceneCollider.h"
#include "ColliderObjects.h"
#include "YAMLOverloads.h"

#include <iostream>

static std::string ColliderTypeToString(ColliderType colliderType)
{
	switch (colliderType)
	{
	case ColliderType::Sphere:	return "Sphere";
	case ColliderType::AABB:	return "AABB";
	case ColliderType::Mesh:	return "Mesh";
	default:
		std::cout << "No conversion for collider type " << (int)colliderType << std::endl;
		break;
	}
}

static ColliderType ColliderTypeFromString(const std::string& s)
{
	if (s == "Sphere")		return ColliderType::Sphere;
	else if (s == "AABB")	return ColliderType::AABB;
	else if (s == "Mesh")	return ColliderType::Mesh;

	std::cout << "No string conversion for collider type " << s << std::endl;
	return ColliderType::None;
}

SceneCollider::SceneCollider(Ref<Mesh> mesh, float bounciness)
	: colliderType(ColliderType::Mesh), 
	collider(CreateRef<MeshCollider>(mesh->GetVertices(), mesh->GetFaces())),
	bounciness(bounciness)
{

}

SceneCollider::SceneCollider(ColliderType colliderType, Ref<void> collider, float bounciness)
	: colliderType(colliderType), collider(collider), bounciness(bounciness)
{

}

void SceneCollider::Save(YAML::Emitter& emitter) const
{
	emitter << YAML::BeginMap;
	emitter << YAML::Key << "ColliderType" << YAML::Value << ColliderTypeToString(colliderType);
	emitter << YAML::Key << "Bounciness" << YAML::Value << bounciness;
	if (colliderType == ColliderType::Sphere)
	{
		emitter << YAML::Key << "Radius" << YAML::Value << std::static_pointer_cast<SphereCollider>(collider)->radius;
	}
	else if (colliderType == ColliderType::AABB)
	{
		emitter << YAML::Key << "Min" << YAML::Value << std::static_pointer_cast<AABBCollider>(collider)->min;
		emitter << YAML::Key << "Max" << YAML::Value << std::static_pointer_cast<AABBCollider>(collider)->max;
	}
	else if (colliderType == ColliderType::Mesh)
	{
		// No need to save anything here because the data we need will be in a mesh object
	}
	emitter << YAML::EndMap;
}

SceneCollider SceneCollider::StaticLoad(Ref<Mesh> mesh, const YAML::Node& node)
{
	ColliderType colliderType = ColliderTypeFromString(node["ColliderType"].as<std::string>());
	float bounciness = node["Bounciness"].as<float>();

	if (colliderType == ColliderType::Sphere)
	{
		Ref<SphereCollider> sphereCollider = CreateRef<SphereCollider>(node["Radius"].as<float>());
		SceneCollider collider(colliderType, sphereCollider, bounciness);
		return collider;
	}
	else if (colliderType == ColliderType::AABB)
	{
		glm::vec3 min = node["Min"].as<glm::vec3>();
		glm::vec3 max = node["Max"].as<glm::vec3>();

		Ref<AABBCollider> aabbCollider = CreateRef<AABBCollider>(min, max);
		SceneCollider collider(colliderType, aabbCollider, bounciness);
		return collider;
	}
	else if (colliderType == ColliderType::Mesh)
	{
		Ref<MeshCollider> meshCollider = CreateRef<MeshCollider>(mesh->GetVertices(), mesh->GetFaces());
		SceneCollider collider(colliderType, meshCollider, bounciness);
		return collider;
	}

	std::cout << "Could not load SceneCollider" << std::endl;
	return SceneCollider(ColliderType::None, nullptr, 0.0f);
}