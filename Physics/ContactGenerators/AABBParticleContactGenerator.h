#pragma once

#include "IParticleContactGenerator.h"
#include "ColliderObjects.h"

class AABBParticleContactGenerator : public IParticleContactGenerator
{
public:
	AABBParticleContactGenerator(Ref<AABBCollider> collider, const glm::mat4& transform, float bounciness);
	AABBParticleContactGenerator(const glm::vec3& minPoint, const glm::vec3& maxPoint, const glm::mat4& transform, float bounciness);
	~AABBParticleContactGenerator() = default;

	virtual size_t AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const override;

private:
	Ref<AABBCollider> collider;
	glm::mat4 transform;
	float bounciness;
};