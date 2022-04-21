#include "AABBParticleContactGenerator.h"

AABBParticleContactGenerator::AABBParticleContactGenerator(Ref<AABBCollider> collider, const glm::mat4& transform, float bounciness)
	: collider(collider),
	transform(transform),
	bounciness(bounciness)

{

}

AABBParticleContactGenerator::AABBParticleContactGenerator(const glm::vec3& minPoint, const glm::vec3& maxPoint, const glm::mat4& transform, float bounciness)
	: collider(CreateRef<AABBCollider>(minPoint, maxPoint)),
	transform(transform),
	bounciness(bounciness)
{

}

size_t AABBParticleContactGenerator::AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const
{
	size_t contactsGenerated = 0;
	for (Particle* particle : particles)
	{
		float radius = particle->GetRadius();
		glm::vec3 particlePos = particle->GetPosition();

		glm::vec4 transformedMin = transform * glm::vec4(collider->min, 1.0f);
		glm::vec4 transformedMax = transform * glm::vec4(collider->max, 1.0f);

		// Find the closest AABB point to our particle
		float closestX = std::max(transformedMin.x, std::min(particlePos.x, transformedMax.x));
		float closestY = std::max(transformedMin.y, std::min(particlePos.y, transformedMax.y));
		float closestZ = std::max(transformedMin.z, std::min(particlePos.z, transformedMax.z));

		glm::vec3 closestPoint(closestX, closestY, closestZ);
		glm::vec3 difference = particlePos - closestPoint;

		// Since square roots are expensive, just compare everything in its squared version
		float distanceSquared = (difference.x * difference.x) + (difference.y * difference.y) + (difference.z * difference.z);
		float radiusSquared = radius * radius;

		if (distanceSquared < radiusSquared) // We have collided!
		{
			// Find contact normal
			glm::vec3 center = (collider->min + collider->max) / 2.0f;
			glm::vec3 normal = particlePos - center;
			if (std::abs(normal.x) > std::abs(normal.y) && std::abs(normal.x) > std::abs(normal.z)) // Down the x axis
			{
				normal.y = 0.0f;
				normal.z = 0.0f;
			}
			else if (std::abs(normal.y) > std::abs(normal.x) && std::abs(normal.y) > std::abs(normal.z))
			{
				normal.x = 0.0f;
				normal.z = 0.0f;
			}
			else if (std::abs(normal.z) > std::abs(normal.y) && std::abs(normal.z) > std::abs(normal.x))
			{
				normal.x = 0.0f;
				normal.y = 0.0f;
			}
			glm::normalize(normal);

			contactInstance->contactNormal = normal;
			contactInstance->p1 = particle;
			contactInstance->p2 = nullptr;
			contactInstance->penetration = radius - sqrt(distanceSquared);
			contactInstance->restitution = particle->GetBounciness() + bounciness;
			contactInstance++; 
			contactsGenerated++;
			if (contactsGenerated > contactLimit)
			{
				return contactsGenerated;
			}
		}
	}

	return contactsGenerated;
}