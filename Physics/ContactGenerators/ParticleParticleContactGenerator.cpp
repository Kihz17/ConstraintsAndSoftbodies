#include "ParticleParticleContactGenerator.h"

size_t ParticleParticleContactGenerator::AddContact(std::vector<Particle*>& particles, ParticleContact* contactInstance, size_t contactLimit) const
{
	if (particles.empty()) // No need to loop
	{
		return 0;
	}

	size_t contactsGenerated = 0;
	for (int i = 0; i < particles.size() - 1; i++)
	{
		for (int j = i + 1; j < particles.size(); j++)
		{
			Particle* particle1 = particles[i];
			Particle* particle2 = particles[j];
			if (particle1 == particle2)
			{
				continue;
			}

			glm::vec3 difference = particle1->GetPosition() - particle2->GetPosition();
			float radiusSum = particle1->GetRadius() + particle2->GetRadius();
			if (glm::dot(difference, difference) < (radiusSum * radiusSum)) // We collided!
			{
				contactInstance->contactNormal = glm::normalize(difference);
				contactInstance->p1 = particle1;
				contactInstance->p2 = particle2;
				contactInstance->penetration = glm::length(difference) - radiusSum;
				contactInstance->restitution = particle1->GetBounciness() + particle2->GetBounciness();
				contactInstance++;
				contactsGenerated++;
				if (contactsGenerated > contactLimit)
				{
					return contactsGenerated;
				}
			}
		}
	}

	return contactsGenerated;
}