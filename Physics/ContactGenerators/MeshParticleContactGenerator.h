#pragma once

#include "IParticleContactGenerator.h"
#include "ColliderObjects.h"
#include "Mesh.h"

class MeshParticleContactGenerator : public IParticleContactGenerator
{
public:
	MeshParticleContactGenerator(Ref<MeshCollider> collider, const glm::mat4& transform, float bounciness);
	MeshParticleContactGenerator(Ref<Mesh> mesh, const glm::mat4& transform, float bounciness);
	~MeshParticleContactGenerator() = default;

	virtual size_t AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const override;

private:
	Ref<MeshCollider> collider;
	glm::mat4 transform;
	float bounciness;
};